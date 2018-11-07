#include "auxfunc.h"

namespace univiewer {

void argParser(std::vector<std::string> &argv,
                        std::vector<std::string> &simple_output_result,
                        std::vector<std::string> &modelFiles,
											  std::vector<std::string> &dispFiles,
											  std::vector<std::string> &contFiles,
											  std::vector<std::string> &nodeDatafiles)
{
	if (argv.empty()) {
		std::cout << "Type 'Univiewer /h' for more help." << std::endl;
		exit(0);
	}
	std::vector<std::string> *ptr_vector_name = NULL;
	for (int i = 0; i<argv.size(); ++i) {

#ifdef __APPLE__
		if (*argv[i] == '-') {
#else
		if ((argv[i][0] == '-') || (argv[i][0] == '/')) {
#endif
			switch (argv[i][1]) {
			case 'm':
				ptr_vector_name = &modelFiles;
				break;
			case 'o':
				ptr_vector_name = &dispFiles;
				break;
			case 'r':
				ptr_vector_name = &simple_output_result;
				break;
			case 's':
				ptr_vector_name = &nodeDatafiles;
				break;
			case 'c':
				ptr_vector_name = &contFiles;
				break;
			case 'h':
				std::cout << std::endl;
				std::cout << "Usage: Univiewer /m file1.xml file2.xml ... fileN.xml /o disp.dat" << std::endl;
				std::cout << std::endl;
				std::cout << "The 'fileN.xml' is a model file that is compatible with VTK API, and other programs. It is a text file using *.vtu format, but you can also add your own data." << std::endl;
				std::cout << "The 'disp.dat' is a data file that contains the displacements of nodes for all the models(file1.xml, file2.xml and so on). It is a binary file with all the data as the type of double, the detail data sequence is as follows:" << std::endl << std::endl;
				std::cout << "#####################################################################################################" << std::endl;
				std::cout << "time1 \n node_1_disp_x node_1_disp_y node_1_disp_z \n node_2_disp_x node_2_disp_y node_2_disp_z \n ... \n node_n_disp_x node_n_disp_y node_n_disp_z" << std::endl;
				std::cout << "time2 \n ......" << std::endl;
				std::cout << "#####################################################################################################" << std::endl << std::endl;
				std::cout << "NOTE: Binary files are not same as txt files which contains symbols like \\n \\t, and all the data saved as String. A Binary file contains data saved as its own type without any symbols like \\n \\t." << std::endl;
				exit(0);
				break;
			default:
				break;
			}
			continue;
		}
		ptr_vector_name->push_back(argv[i]);
		}
}

#ifdef _WIN32
bool OpenFileDlg(std::string &fpathname, std::string &fname) {
  HWND hwnd;
  TCHAR filePathBuffer[MAX_PATH] = { 0 };
  TCHAR fileTitleBuffer[MAX_PATH] = { 0 };
  OPENFILENAME ofn = { 0 };

  hwnd = GetForegroundWindow(); //��ȡǰ̨���ھ�����������е�ǰ̨���ھ��ǿ���̨���ڡ� 
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = hwnd;
  ofn.lpstrFilter = TEXT("simple result(*.dat)\0*.dat\0all type(*.*)\0*.*\0");//Ҫѡ����ļ���׺
  ofn.lpstrInitialDir = NULL;//Ĭ�ϵ��ļ�·��   
  ofn.lpstrFile = filePathBuffer;//����ļ��Ļ�����
  ofn.lpstrFileTitle = fileTitleBuffer;
  ofn.nMaxFile = MAX_PATH;
  ofn.nMaxFileTitle = MAX_PATH;
  ofn.nFilterIndex = 0;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;//��־����Ƕ�ѡҪ����OFN_ALLOWMULTISELECT  
  GetOpenFileName(&ofn);

#ifdef  UNICODE                     // r_winnt
  int len, len1;
  len = WideCharToMultiByte(CP_ACP, 0, filePathBuffer, -1, NULL, 0, NULL, NULL);
  char * ptr1 = new char[len];
  len1 = WideCharToMultiByte(CP_ACP, 0, filePathBuffer, -1, ptr1, len, NULL, NULL);

  len = WideCharToMultiByte(CP_ACP, 0, fileTitleBuffer, -1, NULL, 0, NULL, NULL);
  char * ptr2 = new char[len];
  len1 = WideCharToMultiByte(CP_ACP, 0, fileTitleBuffer, -1, ptr1, len, NULL, NULL);

  fpathname = ptr1;
  fname = ptr2;

  delete ptr1[];
  delete ptr2[];

  return 1;
#else   /* UNICODE */               // r_winnt

  fpathname = filePathBuffer;
  fname = fileTitleBuffer;
  return 1;
#endif /* !_TCHAR_DEFINED */

}
#endif


Mdfile::Mdfile() {};
Mdfile::Mdfile(const std::string &filename) {
	this->Open(filename);
}
Mdfile::~Mdfile() {
	if (infile.is_open())
		this->Close();
};


void Mdfile::Open(const std::string &filename ) {
	infile.open(filename, std::ios::in);
	if (!infile.is_open()) std::cout << "Error in open file: " << filename << std::endl;
}
void Mdfile::Close() {
	infile.close();
}

std::vector<double> Mdfile::GetDoubleArrayFrom(const std::string &title) {
	JumpTo(title);
	std::vector<double> double_list;
	std::string temp_str;
	while (infile >> temp_str) {
		if (temp_str[0] == '#') break;
		double_list.push_back(std::stod(temp_str));
	}
	return double_list;
}

std::vector<int> Mdfile::GetIntArrayFrom(const std::string &title) {
	JumpTo(title);
	std::vector<int> int_list;
	std::string temp_str;
	while (infile >> temp_str) {
		if (temp_str[0] == '#') break;
		int_list.push_back(std::stoi(temp_str));
	}
	return int_list;
}

std::vector<unsigned int> Mdfile::GetUintArrayFrom(const std::string &title) {
	JumpTo(title);
	std::vector<unsigned int> int_list;
	std::string temp_str;
	while (infile >> temp_str) {
		if (temp_str[0] == '#') break;
		int_list.push_back((unsigned int)std::stoi(temp_str));
	}
	return int_list;
}

std::vector<std::string> Mdfile::GetStringFrom(const std::string &title) {
	JumpTo(title);
	std::vector<std::string> str_list;
	std::string temp_str;
	while (infile >> temp_str) {
		if (temp_str[0] == '#') break;
		str_list.push_back(temp_str);
	}
	return str_list;
}

void Mdfile::tryit() {
	infile.seekg(0, std::ios::beg);
	std::string str("");
	infile >> str;

	std::cout<< infile.tellg() <<std::endl;
	std::cout<< str << std::endl;
}

}