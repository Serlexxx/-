#include "BitmapBuffer.h"
#include <algorithm>

/**
* \brief ����������� �� ��������� (�������������� ������ ������, ��� ������);
*/
BitmapBuffer::BitmapBuffer() :width_(0), height_(0), data_(nullptr), pixelAccess_(nullptr) {}

/**
* \brief �����������. �������������� ����� �������� �������� �������� ��� �������� ������
* \param width ������ ������ �����������
* \param height ������ ������ �����������
* \param clearColor ���� �� ���������
*/
BitmapBuffer::BitmapBuffer(const unsigned int width, const unsigned int height, BitmapRGB clearColor) :
	width_(width),
	height_(height),
	data_((width* height) > 0 ? new BitmapDataRGB[width * height] : nullptr),
	pixelAccess_(height > 0 ? new BitmapDataRGB * [height] : nullptr)
{
	if (this->data_)
	{
		const BitmapDataRGB clearColorData = { clearColor.blue,clearColor.green,clearColor.red,0 };
		std::fill_n(this->data_, this->width_ * this->height_, clearColorData);

		for (unsigned int i = 0; i < this->height_; i++) {
			this->pixelAccess_[i] = this->data_ + this->width_ * i;
		}
	}
}

/**
* \brief ����������� �����������. ������������ ����������� ������ data_ � pixelAccess_ ��� ������ �������
* \param other ���������� ������
*/
BitmapBuffer::BitmapBuffer(const BitmapBuffer& other) :
	width_(other.width_),
	height_(other.height_),
	data_(other.data_ ? new BitmapDataRGB[other.width_ * other.height_] : nullptr),
	pixelAccess_(other.pixelAccess_ ? new BitmapDataRGB * [other.height_] : nullptr)
{
	if (other.data_)
	{
		memcpy(this->data_, other.data_, other.width_ * other.height_ * sizeof(BitmapDataRGB));
	}

	if (other.pixelAccess_)
	{
		memcpy(this->pixelAccess_, other.pixelAccess_, other.height_ * sizeof(BitmapDataRGB*));
	}
}

/**
* \brief ����������� �����������. ������������ ����� ������� � ������������ ��������
* \param other R-value ������ �� ������ � ������� ���������� ����� �������
*/
BitmapBuffer::BitmapBuffer(BitmapBuffer&& other) noexcept :BitmapBuffer()
{
	std::swap(this->width_, other.width_);
	std::swap(this->height_, other.height_);
	std::swap(this->data_, other.data_);
	std::swap(this->pixelAccess_, other.pixelAccess_);
}

/**
* \brief �������� ����������-�����������. ������������ ����������� ������ data_ � pixelAccess_ ��� ������ �������
* \param other ������������� ������
* \return ������ �� ������� ������
*/
BitmapBuffer& BitmapBuffer::operator=(const BitmapBuffer& other)
{
	delete[] this->data_;
	delete[] this->pixelAccess_;

	this->width_ = other.width_;
	this->height_ = other.height_;

	if (other.data_ && (other.width_ * other.height_) > 0)
	{
		memcpy(this->data_, other.data_, other.width_ * other.height_ * sizeof(BitmapDataRGB));
		memcpy(this->pixelAccess_, other.pixelAccess_, other.height_ * sizeof(BitmapDataRGB*));
	}

	return *this;
}

/**
* \brief �������� ����������-�����������. ������������ ����� ������� � ������������� ��������
* \param other ������������� ������ (r-value �����)
* \return ������ �� ������� ������
*/
BitmapBuffer& BitmapBuffer::operator=(BitmapBuffer&& other) noexcept
{
	std::swap(this->width_, other.width_);
	std::swap(this->height_, other.height_);
	std::swap(this->data_, other.data_);
	std::swap(this->pixelAccess_, other.pixelAccess_);
	return *this;
}

/**
* \brief �������� ��������� � �������
* \param y ������ ����
* \return ������ �������� ����
*/
BitmapDataRGB* BitmapBuffer::operator[](int y)
{
	return this->pixelAccess_[y];
}

/**
* \brief ������� ������ data_ � ������������ ������� pixelAccess_
*/
BitmapBuffer::~BitmapBuffer()
{
	delete[] this->data_;
	delete[] this->pixelAccess_;
}

/**
* \brief �������� ������ ������
* \return ������ � ������
*/
unsigned BitmapBuffer::GetSize() const
{
	return sizeof(BitmapDataRGB) * this->width_ * this->height_;
}

/**
* \brief ��������� ����� �������� ������
* \param color ���� �������
*/
void BitmapBuffer::Clear(BitmapRGB color)
{
	if (this->data_) {
		const BitmapDataRGB clearColorData = { color.blue,color.green,color.red,0 };
		std::fill_n(this->data_, this->width_ * this->height_, clearColorData);
	}
}

/**
* \brief �������� ������ ������
* \return ��������� �� ������ �������
*/
BitmapDataRGB* BitmapBuffer::GetData() const
{
	return this->data_;
}

/**
* \brief ������ � �������� ������
* \return ��������� �� ������ ���������� �� ������� BitmapDataRGB
*/
BitmapDataRGB** BitmapBuffer::Pixels()
{
	return this->pixelAccess_;
}

/**
* \brief �������� ������
* \return ������ (uint)
*/
unsigned BitmapBuffer::GetWidth() const
{
	return this->width_;
}

/**
* \brief �������� ������
* \return ������ (uint)
*/
unsigned BitmapBuffer::GetHeight() const
{
	return this->height_;
}