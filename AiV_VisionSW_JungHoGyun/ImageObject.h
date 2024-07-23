#pragma once

#define interface class

interface ImageObject
{
public:
	virtual ~ImageObject() {};
	virtual int getWidth() const = 0;					// width ��ȯ
	virtual int getHeight() const = 0;					// height ��ȯ
	virtual unsigned char* getBuffer() const = 0;		// buffer ��ȯ
	virtual void set_image_object(const vector<uchar>& data, int width, int height) = 0;	// �ȼ���, ���� ũ�� ������ �޾� �¾�
};

class imageobject_src : public ImageObject
{
public:
	imageobject_src();									// 5000x5000 ���� �̹��� ����
	imageobject_src(const Mat& mat);					// �ȼ���, ���� ũ�� ������ �޾� �¾�				
	virtual int getWidth() const override;				// width ��ȯ
	virtual int getHeight() const override;				// height ��ȯ
	virtual unsigned char* getBuffer() const override;	// buffer ��ȯ
	virtual void set_image_object(const vector<uchar>& data, int width, int height) override;	// �ȼ���, ���� ũ�� ������ �޾� �¾�
private:
	unsigned char* buffer;	// pixel ���� ���Ե� buffer
	int width;				// ���� ũ��(width)
	int height;				// ���� ũ��(height)
};

class imageobject_dst : public ImageObject
{
public:
	imageobject_dst();
	virtual int getWidth() const override;				// width ��ȯ
	virtual int getHeight() const override;				// height ��ȯ
	virtual unsigned char* getBuffer() const override;	// buffer ��ȯ
	virtual void set_image_object(const vector<uchar>& data, int width, int height) override;	// �ȼ���, ���� ũ�� ������ �޾� �¾�
private:
	unsigned char* buffer;	// pixel ���� ���Ե� buffer
	int width;				// ���� ũ��(width)
	int height;				// ���� ũ��(height)
};

