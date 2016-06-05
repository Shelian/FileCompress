#ifndef __COMPRESS_H__
#define __COMPRESS_H__
#include "Haffman.h"
#include<string>

template<class T>
struct BinaryTreeNode
{
	T _value;
	BinaryTreeNode<T>* _left;
	BinaryTreeNode<T>* _right;

	BinaryTreeNode(const T& value)
		:_value(value)
		, _left(NULL)
		, _right(NULL)
	{}
};





typedef long long LongType;

struct CharInfo
{
	unsigned char _ch;
	LongType _count;
	string _code;

	CharInfo(LongType count = 0)
		:_count(count)
		, _ch(0)
	{}

	LongType operator+(const CharInfo& c2)const
	{
		return _count + c2._count;
	}

	bool operator!=(const CharInfo& c2)const
	{
		return _count != c2._count;
	}

	bool operator<(const CharInfo& c2)const
	{
		return _count < c2._count;
	}
};

template<class T>
class FileCompress
{
public:
	FileCompress(char* CompressFile)
	{
		for (size_t i = 0; i < 256; i++)
		{
			_infos[i]._ch = i;
			_infos[i]._count = 0;
		}

		//统计文件中字符出现的次数
		FILE* fOut = fopen(CompressFile, "rb");
		assert(fOut);
		FILE* fIn = fopen("Output.txt", "wb");
		assert(fIn);

		char ch = fgetc(fOut);
		while (!feof(fOut))
		{
			_infos[(unsigned char)ch]._count++;
 			ch = fgetc(fOut);
		}

		
		//生成哈夫曼树
		CharInfo invalid(0);
		Haffman<CharInfo> hfm(_infos, 256,invalid);

		//生成哈夫曼编码
		string code;
		GenerateHuffmanCode(hfm.GetRoot(), code);//生成的哈夫曼编码的code在哈夫曼树中原来的文件中的code没有修改
		

		//写入文件，读取哈夫曼树
		fseek(fOut, 0, SEEK_SET);
		ch = fgetc(fOut);
		int pos = 0;
		unsigned char value = 0;
		while (!feof(fOut))
		{
			string& code = _infos[(unsigned char)ch]._code;
			for (size_t j = 0; j < _infos[(unsigned char)ch]._code.size(); j++)
			{
				//每次进来先移一位，第一次进来为0移动没有关系
				value <<= 1;
				if (code[j] =='1')
					value |= 1;
				
				//满8个位写一次文件
				if (++pos == 8)
				{
					pos = 0;
					fputc(value, fIn);
					value = 0;
				}

			}
			ch = fgetc(fOut);
		}
		 //如果剩余的值不够八位，往左移空余的位数
		if (pos > 0)
		{
			value <<= (8 - pos);
			fputc(value, fIn);
		}

		//写配置文件
		//a,4
		//b,3
		//c,2
		//d.1
		FILE* filewrite = fopen("configFile.txt", "wb");//遍历哈夫曼树，为根节点的，把ch和count写入，后序遍历
		string infoStr;
		char countStr[20];
		for (int i = 0; i < 256; ++i)
		{
			if (_infos[i] != invalid)
			{
				// 如果遇到换行符，则连写两个换行符
				/*if (_fileInfos[i]._ch == 10)
				{
				infoStr = _fileInfos[i]._ch;
				}*/

				infoStr = _infos[i]._ch;
				infoStr += ',';
				_itoa(_infos[i]._count, countStr, 10);//一定要使用itoa因为对于打文件一个字符是存不下count数量的
				infoStr += countStr;
				infoStr += '\n';

				fputs(infoStr.c_str(), filewrite);
			}
		}
		/*WriteConfigFile(filewrite, hfm.GetRoot());*/

		fclose(fIn);
		fclose(fOut);
		fclose(filewrite);

	}

	//写配置文件
	//void WriteConfigFile(FILE* filewrite,BinaryTreeNode<CharInfo>* root)
	//{
	//	if (root == NULL)
	//		return;
	//	string infoStr;
	//	char countStr[20];
	//	if (root->_left != NULL)
	//		WriteConfigFile(filewrite,root->_left);
	//	if (root->_right != NULL)
	//		WriteConfigFile(filewrite,root->_right);

	//	if (root->_left == NULL  && root->_right == NULL)
	//	{
	//		infoStr = root->_value._ch;
	//		infoStr += ',';
	//		_itoa(root->_value._count, countStr, 10);
	//		infoStr += countStr;
	//		infoStr += '\n';

	//		fputs(infoStr.c_str(), filewrite);
	//		/*fputc(root->_value._ch, filewrite);
	//		fputc(',', filewrite);
	//		fputc(root->_value._count + '0', filewrite);
	//		fputc('\n', filewrite);*/
	//	}
	//	
	//}

	//我真是个脑残，文件只有关闭以后才能看到效果，两个小时啊，真的坑
	void GenerateHuffmanCode(BinaryTreeNode<CharInfo>* root, string code)
	{
		if (root == NULL)
			return;

		if (root->_left != NULL)
		GenerateHuffmanCode(root->_left,code+'0');
		if (root->_right != NULL)
		GenerateHuffmanCode(root->_right, code + '1');

		if (root->_left == NULL&&root->_right == NULL)
		{
			root->_value._code = code;
			_infos[root->_value._ch]._code = code;//给没一个数的code置成编码的形式，这样就不需要读取数据，再去找哈夫曼编码了
			cout << _infos[root->_value._ch]._ch << _infos[root->_value._ch]._code << endl;
		}
		
	}

private:
	CharInfo _infos[256];
};


template<class T>
class UnCompress
{
public:
	UnCompress(const char * CompressFilename,const char * Config)
	{
		FILE* fOut1 = fopen(CompressFilename, "rb");
		FILE* fConfig = fopen(Config, "rb");
		FILE* fWrite = fopen("fWrite.txt", "wb");
		assert(fOut1);
		//重建哈夫曼树
		//先读取配置文件，还原到_infos中去，然后重建哈夫曼树
		//解压缩，根节点的次数
		
		string str;
		while (ReadLine(fConfig, str))
		{
			//对\n做处理
			if (str.empty())
			{
				str += '\n';
			}
			else
			{
				_infos[(unsigned char)str.c_str()[0]]._ch = (unsigned char)str.c_str()[0];
				_infos[(unsigned char)str.c_str()[0]]._count = atoi(str.substr(2).c_str());
				str.clear();
			}
			
		}

		//生成哈夫曼树
		CharInfo invalid(0);
		Haffman<CharInfo> hfm(_infos, 256, invalid);
		//重建好哈夫曼树
		char ch= fgetc(fOut1);
		long long i = hfm.GetRoot()->_value._count;//i=10
		
		//压缩的时候压了多少个字符，解压就解压多少个字符
		BinaryTreeNode<CharInfo>* root = hfm.GetRoot();
		//int pos = 8;
		//BinaryTreeNode<CharInfo>* cur = root;
		//while (!feof(fOut1))
		//{
		//	while (cur && cur->_left && cur->_right)
		//	{
		//		if (pos == 0)
		//		{
		//			pos = 8;
		//			ch = fgetc(fOut1);
		//			if (feof(fOut1))
		//				break;
		//		}

		//		--pos;

		//		if (ch & 1 << pos)
		//		{
		//			cur = cur->_right;
		//			cout << "1";
		//		}
		//		else
		//		{
		//			cur = cur->_left;
		//			cout << "0";
		//		}
		//	}

		//	if (cur && !feof(fOut1))
		//	{
		//		cout << "->";
		//		fputc(cur->_value._ch, fWrite);
		//		cur = root;
		//	}
		//}
		while (i>0)
		{
			//将字符转换成string
			unsigned char value = (unsigned char)ch;
			int pos = 7;
			
			while (pos >=0 && i>0)
			{
				if (((value>>pos)&1) == 0)
				{
					root = root->_left;
				}
				else
				{
					root = root->_right;
				}

				if (root->_left == NULL && root->_right == NULL)
				{
					fputc(root->_value._ch, fWrite);
					root = hfm.GetRoot();
					i--;
				}
				
				pos--;
				
			}
			ch=fgetc(fOut1);
		}
		fclose(fOut1);
		fclose(fConfig);
		fclose(fWrite);
	}

	bool ReadLine(FILE* filename,string& str)
	{
		
		char ch = fgetc(filename);
		if (feof(filename))
			return false;

		while (!feof(filename) && ch != '\n')
		{
			str += ch;
			ch = fgetc(filename);
		}

		return true;
	}

private:
	CharInfo _infos[256];

};


void testCompress()
{
	FileCompress<CharInfo> fc("Input.BIG");
	UnCompress<CharInfo> ucp("Output.txt","configFile.txt");
}





#endif //__COMPRESS_H__