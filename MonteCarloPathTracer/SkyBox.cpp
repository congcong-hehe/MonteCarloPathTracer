#define _CRT_SECURE_NO_WARNINGS

#include "SkyBox.h"
#include "external/rgbe.h"
#include <stdio.h>
#include <vector>
#include <algorithm>
#include "Utility.h"
#include "Image.h"

using namespace std;

// https://blog.csdn.net/weixin_44176696/article/details/119988866?spm=1001.2014.3001.5502
SkyBox::SkyBox(const char* file_name)
{
    FILE* f = fopen(file_name, "rb");
    RGBE_ReadHeader(f, &width_, &height_, NULL);
    image_ = (float*)malloc(sizeof(float) * 3 * width_ * height_);
    RGBE_ReadPixels_RLE(f, image_, width_, height_);

    if (image_ == nullptr) return;

    resolution = std::max(width_, height_);

    // 计算纹理的亮度分布
    float sum_lum = 0.f;
    vector<vector<float>> pdf(height_);
    for_each(pdf.begin(), pdf.end(), [&](vector<float>& v) { v.resize(width_); });
    for (int i = 0; i < height_; ++i)
    {
        for (int j = 0; j < width_; ++j)
        {
            int index = 3 * (width_ * i + j);
            float r = image_[index + 0];
            float g = image_[index + 1];
            float b = image_[index + 2];
            float lum = r * 0.299f + g * 0.587f + b * 0.114f;
            pdf[i][j] = lum;
            sum_lum += lum;
        }
    }
    for (int i = 0; i < height_; ++i)
        for (int j = 0; j < width_; ++j)
            pdf[i][j] /= sum_lum;   // 归一化处理

    // 计算x的概率密度函数
    vector<float> x_pdf(width_, 0.0f);
    for (int i = 0; i < width_; ++i)
        for (int j = 0; j < height_; ++j)
            x_pdf[i] += pdf[j][i];

    // 计算x的概率分布函数
    vector<float> x_cdf = x_pdf;
    for (int i = 1; i < width_; ++i)
        x_cdf[i] += x_cdf[i - 1];

    // 计算y在X = x下的条件概率
    vector<vector<float>> pdf_y_condition = pdf;
    for (int i = 0; i < width_; ++i)
        for (int j = 0; j < height_; ++j)
            pdf_y_condition[j][i] /= x_pdf[i];

    // 计算y在X=x下的条件概率分布函数
    vector<vector<float>> cdf_y_condition = pdf_y_condition;
    for (int i = 1; i < height_; ++i)
        for (int j = 0; j < width_; ++j)
            cdf_y_condition[i][j] += cdf_y_condition[i - 1][j];

    // cdf_y_condition转置,第i行表示y在X=i下的条件概率分布函数
    vector<vector<float>> temp = cdf_y_condition;
    cdf_y_condition = vector<vector<float>>(width_);
    for (auto& v : cdf_y_condition) v.resize(height_);
    for (int i = 0; i < width_; ++i)
        for (int j = 0; j < height_; ++j)
            cdf_y_condition[i][j] = temp[j][i];

    sample_x.resize(height_);
    for (auto& v : sample_x) v.resize(width_);
    sample_y.resize(height_);
    for (auto& v : sample_y) v.resize(width_);
    sample_p.resize(height_);
    for (auto& v : sample_p) v.resize(width_);

    // 穷举x1, x2预计算x,y。最后我们使用两个随机数作为概率，就可以得到需要取的像素的位置
    for(int i = 0; i < height_; ++i)
        for (int j = 0; j < width_; ++j)
        {
            float x1 = float(i) / height_;
            float x2 = float(j) / width_;

            int x = lower_bound(x_cdf.begin(), x_cdf.end(), x1) - x_cdf.begin();
            int y = lower_bound(cdf_y_condition[x].begin(), cdf_y_condition[x].end(), x2) - cdf_y_condition[x].begin();

            sample_x[i][j] = x;
            sample_y[i][j] = y;
            sample_p[i][j] = pdf[i][j];
        }

    /*Image pdf_img;
    pdf_img.setWidth(width_);
    pdf_img.setHeight(height_);
    pdf_img.init();

    for (int i = 0; i < height_; ++i)
        for (int j = 0; j < width_; ++j)
        {
            pdf_img.setColor(i, j, Color(pdf[i][j] * 500000.0f));
        }

    pdf_img.write("../image/pdf.png");*/

}

SkyBox::~SkyBox()
{
    delete image_;
}
Color SkyBox::getColor(const int u, const int v)
{
    int index = u * width_ + v;
    return Color(image_[3 * index + 0], image_[3 * index + 1], image_[3 * index + 2]);
}

Color SkyBox::sample(Ray& ray)
{
    float phi = 0.5f - std::atan2(-ray.direction.z, -ray.direction.x) / PI * 0.5f;
    float theta = 0.5f - std::asin(ray.direction.y) / PI;

    int width_index = phi * width_;
    int height_index = theta * height_;
    int index = height_index * width_ + width_index;

    return getColor(height_index, width_index);
}

float SkyBox::hdrpdf(Vec& dir, Color& color)
{
    float phi = 0.5f - std::atan2(-dir.z, -dir.x) / PI * 0.5f;
    float theta = 0.5f - std::asin(dir.y) / PI;

    int width_index = phi * width_;
    int height_index = theta * height_;

    color = getColor(height_index, width_index);
    return sample_p[height_index][width_index] * float(resolution * resolution / 2) / (2.0f * PI * PI * sin(theta));;
}

Color SkyBox::hdrSample(Vec& dir, int& x, int& y, float &pdf)
{
    float x1 = getRandIntNum(0, height_ - 1);   // 注意-1
    float x2 = getRandIntNum(0, width_ - 1);
    x = sample_x[x1][x2];
    y = sample_y[x1][x2];

    // 计算方向
    float theta = (0.5f - (float)y / height_) * PI, phi = (0.5f - (float)x / width_) * 2 * PI;
    dir.y = sin(theta);
    dir.x = -cos(theta) * cos(phi);
    dir.z = -cos(theta) * sin(phi);
    pdf = sample_p[y][x] * float(resolution * resolution / 2) / (2.0f * PI * PI * sin(theta));

    return getColor(y, x);
}
