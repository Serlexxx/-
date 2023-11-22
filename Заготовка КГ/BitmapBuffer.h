#pragma once

/**
 * \brief ��������� ����������� ����. ������������ � �������� � ������������� �����������, �.�
 * ������ �������, ������� ������������ ��� ������ ���������� WinAPI, ������������
 * �� ���� ������ ������ ��������. ������� ���������� ����� (������) � ��������� �����.
 */
struct BitmapDataRGB
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char reserved;
};

/**
 * \brief ��������� ����������� ����. ������������ ��� ������������� ������� ������ � �� ����� �������
 * ��������� ������� ������ � �������� �� ����������. �� BitmapDataRGB ����������, �� ����, ������
 * �������� (�������� ������ � ���������� 4-�� �����). ���� ������ ��������� � �������� ���������� (RGB - ����� ��������)
 */
struct BitmapRGB
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

/**
 * \brief ����� ������ ������ �����������
 */
class BitmapBuffer
{
private:
	unsigned int width_;           // ������ �����������
	unsigned int height_;          // ������ �����������
	BitmapDataRGB* data_;         // ������ � ������ ��������
	BitmapDataRGB** pixelAccess_; // ������ ���������� �� "���-�������" � data_ (������ ��� � 2-������ �������)

public:

	/**
	 * \brief ����������� �� ��������� (�������������� ������ �����, ��� ������);
	 */
	BitmapBuffer();

	/**
	 * \brief �����������. �������������� ����� �������� �������� �������� ��� �������� ������
	 * \param width ������ ������ �����������
	 * \param height ������ ������ �����������
	 * \param clearColor ���� �� ���������
	 */
	BitmapBuffer(const unsigned int width, const unsigned int height, BitmapRGB clearColor = { 0,0,0 });

	/**
	 * \brief ����������� �����������. ������������ ����������� ������ data_ � pixelAccess_ ��� ������ �������
	 * \param other ���������� ������
	 */
	BitmapBuffer(const BitmapBuffer& other);

	/**
	 * \brief ����������� �����������. ������������ ����� ������� � ������������ ��������
	 * \param other R-value ������ �� ������ � ������� ���������� ����� �������
	 */
	BitmapBuffer(BitmapBuffer&& other) noexcept;

	/**
	 * \brief �������� ����������-�����������. ������������ ����������� ������ data_ � pixelAccess_ ��� ������ �������
	 * \param other ������������� ������
	 * \return ������ �� ������� ������
	 */
	BitmapBuffer& operator=(const BitmapBuffer& other);

	/**
	 * \brief �������� ����������-�����������. ������������ ����� ������� � ������������� ��������
	 * \param other ������������� ������
	 * \return ������ �� ������� ������
	 */
	BitmapBuffer& operator=(BitmapBuffer&& other) noexcept;

	/**
	 * \brief �������� ��������� � �������
	 * \param y ������ ����
	 * \return ������ �������� ����
	 */
	BitmapDataRGB* operator[](int y);

	/**
	 * \brief ������� ������ data_ � ������������ ������� pixelAccess_
	 */
	~BitmapBuffer();

	/**
	 * \brief �������� ������ ������
	 * \return ������ � ������
	 */
	unsigned int GetSize() const;

	/**
	 * \brief ��������� ����� �������� ������
	 * \param color ���� �������
	 */
	void Clear(BitmapRGB color);

	/**
	 * \brief �������� ������ ������
	 * \return ��������� �� ������ �������
	 */
	BitmapDataRGB* GetData() const;

	/**
	 * \brief ������ � �������� ������
	 * \return ��������� �� ������ ���������� �� ������� BitmapDataRGB
	 */
	BitmapDataRGB** Pixels();

	/**
	 * \brief �������� ������
	 * \return ������ (uint)
	 */
	unsigned int GetWidth() const;

	/**
	 * \brief �������� ������
	 * \return ������ (uint)
	 */
	unsigned int GetHeight() const;
};