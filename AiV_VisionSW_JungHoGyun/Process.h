#pragma once
#include <string>
#include <vector>
#include <memory> // ����Ʈ ������ ����� ���� �ʿ�

class imageobject_src;
class imageobject_dst;
namespace std 
{
	class mutex;
}

class Process
{
public:
	Process();	// ������, ���μ��� ����
	~Process();	// �Ҹ���, ������,�޸� ����

private:
	std::string img_path;				// �̹��� ���
	int kernel;							// Ŀ�� ��
	int num_threads;					// ������ ��
	std::string resultimg_path;			// ��� �̹��� ���� ���
	std::vector<imageobject_src*> src;	// ���� �̹��� ��ü
	std::unique_ptr<std::mutex> mtx;
	bool initialize();	// �ʱ�ȭ. ���α׷� ���� �� ���� ���� ���� �б�
	void thread_Start();// ������ ����

	void Start(int i);																// New_processImage ����
	bool New_processImage(imageobject_src* src);									// �̹��� �¾�
	bool CompareImage(const imageobject_dst& dst1, const imageobject_dst& dst2);	// �̹��� ��
	void SaveImage(const imageobject_dst& dst1, const imageobject_dst& dst2);		// �̹��� ����
};

