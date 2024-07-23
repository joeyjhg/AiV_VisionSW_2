#include "pch.h"
#include "CustomBlur.h"
#include "ImageObject.h"
#include "Logging.h"

#define EXPORTDLL extern "C" __declspec(dllexport)
namespace Custom {
	// ���� ���� ���� ���ϱ�
	int Integral_Sum(const vector<vector<int>>& integralImage, int x1, int y1, int x2, int y2) {
		int A = (x1 > 0 && y1 > 0) ? integralImage[x1 - 1][y1 - 1] : 0;
		int B = (x1 > 0) ? integralImage[x1 - 1][y2] : 0;
		int C = (y1 > 0) ? integralImage[x2][y1 - 1] : 0;
		int D = integralImage[x2][y2];
		return D - B - C + A;
	}
	// Bulr ó�� Custom �Լ�
	bool ImageBlur(const ImageObject* src, ImageObject* dst, const int kernelSize) {

		writeFile("image_blur.log", "Custom bulr() Start");
		// Ŀ���� ¦���ų� 1���� �۴ٸ�
		if (kernelSize % 2 == 0 || kernelSize < 1)
			return false;

		int height = src->getHeight();
		int width = src->getWidth();
		const unsigned char* srcBuffer = src->getBuffer();

		// �̹��� ũ�� ��� - default ���� BORDER_REFLECT_101�� ����ϱ� ���� (kernel/2)*2 ��ŭ ����, ���α��� �ø�
		int halfKernel = kernelSize / 2;
		int paddedWidth = width + 2 * halfKernel;
		int paddedHeight = height + 2 * halfKernel;

		// �е��� ������ �Ҵ�
		vector<vector<unsigned char>> paddedData(paddedHeight, vector<unsigned char>(paddedWidth, 0));


		// ���� �����͸� �߾ӿ� ����
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				paddedData[i + halfKernel][j + halfKernel] = srcBuffer[i * width + j];


		// ��� ó�� - BORDER_REFLECT_101�� ��谪�� ���ʿ� �ִ� ���� ��Ī�ǵ��� �ؾ���
		for (int i = 0; i < halfKernel; ++i)
			for (int j = 0; j < width; ++j)
			{
				paddedData[i][j + halfKernel] = srcBuffer[(halfKernel - i) * width + j];						// ��� �е�
				paddedData[height + halfKernel + i][j + halfKernel] = srcBuffer[(height - 2 - i) * width + j];  // �ϴ� �е�
			}


		for (int i = 0; i < height; ++i)
			for (int j = 0; j < halfKernel; ++j)
			{
				paddedData[i + halfKernel][j] = srcBuffer[i * width + (halfKernel - j)];						// ���� �е�
				paddedData[i + halfKernel][width + halfKernel + j] = srcBuffer[i * width + (width - 2 - j)];	// ���� �е�
			}

		for (int i = 0; i < halfKernel; ++i)
			for (int j = 0; j < halfKernel; ++j)
			{
				paddedData[i][j] = srcBuffer[(halfKernel - i) * width + (halfKernel - j)];											// �»�� �е�
				paddedData[i][width + halfKernel + j] = srcBuffer[(halfKernel - i) * width + (width - 2 - j)];						// ���� �е�
				paddedData[height + halfKernel + i][j] = srcBuffer[(height - 2 - i) * width + (halfKernel - j)];					// ���ϴ� �е�
				paddedData[height + halfKernel + i][width + halfKernel + j] = srcBuffer[(height - 2 - i) * width + (width - 2 - j)];// ���ϴ� �е�
			}


		// ���� �̹��� ���
		vector<vector<int>> integralImage(paddedHeight, vector<int>(paddedWidth, 0));

		for (int i = 0; i < paddedHeight; ++i) 
		{
			int rowSum = 0;
			for (int j = 0; j < paddedWidth; ++j) 
			{
				rowSum += paddedData[i][j];
				integralImage[i][j] = rowSum + (i > 0 ? integralImage[i - 1][j] : 0);
			}
		}
		// �� ó���� �����͸� ���� �޸� �Ҵ�
		unsigned char* dstBuffer = new unsigned char[width * height];

		// �� ó��
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j) 
			{
				int x1 = i;
				int y1 = j;
				int x2 = i + 2 * halfKernel;
				int y2 = j + 2 * halfKernel;

				int sum = Integral_Sum(integralImage, x1, y1, x2, y2);
				int area = (x2 - x1 + 1) * (y2 - y1 + 1);
				dstBuffer[i * width + j] = static_cast<unsigned char>(round(sum / float(area)));
			}
		

		// dst�� ������ ����
		dst->set_image_object(vector<unsigned char>(dstBuffer, dstBuffer + width * height), width, height);
		delete[] dstBuffer;

		writeFile("image_blur.log", "Custom bulr() Complete");

		return true;
	}

	bool return_ImageBlur_Custom(const ImageObject* src, ImageObject* dst, const int kernelSize)
	{
		return Custom::ImageBlur(src, dst, kernelSize);
	}

}
