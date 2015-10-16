///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//	The barco_launch package aims to provide tools to easily                 //
//  calibrate, start and debug the autobotz boat                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//
// O programa roda nos seguintes modos:
// 		nenhum argumento:  Os nós do ROS são iniciados com as informações
//							disponíveis em um arquivo de configuração.
//		
//		arduinos: Pede para que um arduino seja plugado por vez, configurando
//                        assim que porta corresponde a cada um.
//
//		rec: inicia o barco normalemnte mas grava todos os tópicos usando rosbag
//
//		calibra-us: roda procedimento de calibração dos ultrassons e grava os
//					 os resultados em um arquivo
//
//		calibra-imu: roda procedimento de calibração da imu

#include <cstdio>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "tinydir.h"  //biblioteca para acessar diretorios
using namespace std;

/////////////// parametros /////////////////
const char* devDir            = "/dev/";  //diretorio de dispositivos do linux
const char* arduino_substring = "ACM"; //substring que difere um arduino dos outros tty

const char* arquivo_conf_arduino = "visao/arduinos.conf";  //arquivo que guarda em qual porta 
                                                            //esta cada arduino

////////////////////////////////////////////

//funcoes correspondentes aos modos de operacao
void launch(bool rec);  //inicia barco, se rec == true, grava dados com rosbag
void arduinos();      //identifica a porta de cada arduino
void calibra_us();    //calibra ultrassonico
void calibra_imu();   //calibra a imu

//funcoes auxiliares
vector < string > listaArquivos(const char* dir);
vector < string > achaNovosElementos(vector < string > antigo, vector < string > novo);
vector < string > filtraPorSubstring(vector <string> in, const char* mask);

string conectaArduino(const char* nome_legivel);

int main(int argc, char** argv)
{
	//////// tratar os argumentos passados ao programa
	char arg_rec    [] = "rec"; 	//strings de argumentos correspondentes aos modos
	char arg_cal_us [] = "calibra-us";
	char arg_cal_imu[] = "calibra-imu";
	char arg_ardu   [] = "arduino";

	bool has_rec     = false;  //flags de quais argumentos estão presentes
	bool has_cal_us  = false;
	bool has_cal_imu = false;
	bool has_ardu    = false;

	//search for arguments
	for(int i = 1; i < argc; i++)
	{
		if(strcmp(arg_rec, argv[i]) == 0)
			has_rec = true;
		else if(strcmp(argv[i], arg_cal_us) == 0)
			has_cal_us = true;
		else if(strcmp(argv[i], arg_cal_imu) == 0)
			has_cal_imu = true;
		else if(strcmp(argv[i], arg_ardu) == 0)
			has_ardu = true;
	}
			
	//choose mode
	if(argc == 1)   //no params
		launch(has_rec);
	else if(has_ardu)
		arduinos();
	else if(has_cal_us)
		calibra_us();
	else if(has_cal_imu)
		calibra_imu();
	else
		printf("barco_launch: Nenhum argumento valido...\n");

	return 0;
}

void arduinos()
{

	//////////// adquirir endereço dos arduinos //////////////
	cout << "\nDesconecte todos os arduinos e pressione enter...";
	cin.get();

	//strings com os enderecos de cada arduino
	string ardu_prop;
	string ardu_som;
	string ardu_col;
	string ardu_imu;

	//pede para o usuario conectar cada um dos arduinos
	ardu_prop = conectaArduino("TURBino (ponte H + radio)"               );
	ardu_som  = conectaArduino("arduSom (ultrassonicos + base giratoria)");
	ardu_col  = conectaArduino("arduCol (caracol + garra + botao inicio)");
	ardu_imu  = conectaArduino("arduIMU"                                 );

	//reporta o resultado
	cout << "\nArduinos configurados com os arquivos: " << endl;
	cout << "TURBino: " << ardu_prop << endl;
	cout << "arduSom: " << ardu_som << endl;
	cout << "arduCol: " << ardu_col << endl;
	cout << "arduIMU: " << ardu_imu << endl;

	//gravar arquivo de configuracao
	cout << "\nGravando no arquivo " << arquivo_conf_arduino << endl;

	ofstream fout;
	fout.open(arquivo_conf_arduino);
	if(!fout.is_open() || !fout.good())
	{
		cout << "ERRO: gravar arquivo de saida falhou.\n Working dir is:" << endl;
		system("pwd");
	}

	fout << ardu_prop << endl;
	fout << ardu_som  << endl;
	fout << ardu_col  << endl;
	fout << ardu_imu  << endl;

	fout.close();

	cout << "Pronto!\n" << endl;
}

void launch(bool rec)
{
	
}

void calibra_us()
{
	
}

void calibra_imu()
{
	
}

///////////////////////////////////////////////////////////////
//                   Funcoes auxiliares                      //
///////////////////////////////////////////////////////////////

//retorna uma lista com todos os arquivos de um diretorio
//ignora diretorios
vector < string > listaArquivos(const char* dir_name)
{
	tinydir_dir dir;  //da biblioteca tinydir
	vector < string > out;
	string fname;

	if (tinydir_open(&dir, dir_name) == -1)  //abrir diretorio
	{
		cerr << "Não foi possível acessar o diretorio " << dir_name << endl;
		return out;
	}

	while (dir.has_next) //pegar cada arquivo no diretorio
	{
		tinydir_file file;
		if (tinydir_readfile(&dir, &file) == -1)
			cerr << "Erro ao tentar ler arquivos em " << dir_name << endl;

		if(!file.is_dir)
		{
			fname = file.name;
			out.push_back(fname.c_str());
		}

		//avanca para o proximo arquivo
		tinydir_next(&dir);
	}
}

//retorna somente as strings que contem uma substring
vector < string > filtraPorSubstring(vector <string> in, const char* mask)
{
	vector < string > out;

	for(int i = 0; i < in.size(); i++)
	{
		if(strstr(in[i].c_str(), mask) != NULL)  //if mask is a substring of in[i]
			out.push_back(in[i]);
	}

	return out;
}


//retorna um vetor com todos os elementos presentes em novo
//que nao estavam presentes em antigo
vector < string > achaNovosElementos(vector < string > antigo, vector < string > novo)
{
	vector< string > out;

	for(int i = 0; i < novo.size(); i++)
	{
		bool presente = false;

		//procura elemento em antigo
		for(int j = 0; j < antigo.size(); j++)
		{
			if(novo[i] == antigo[j])
			{
				presente = true;
				break;
			}
		}

		//so adiciona à saida se nao estiver em antigo
		if(!presente)
			out.push_back(novo[i]);

	}

	return out;
}

//pede para o usuario conectar um arduino e retorna o arquivo correspondente ao mesmo
string conectaArduino(const char* nome_legivel)
{
	//
	vector < string > tty_list;
	vector < string > tty_list_new;
	vector < string > dif;

	//criar lista de terminais pre-existentes
	tty_list = listaArquivos(devDir);

	//pedir para conectas e achar a diferença
	cout << "Conecte " << nome_legivel << " e pressione enter...";
	cin.get();

	tty_list_new = listaArquivos(devDir);             //pega todos os arquivos do diretorio
	dif = achaNovosElementos(tty_list, tty_list_new); //acha os que nao estavam la antes
	dif = filtraPorSubstring(dif, arduino_substring); //filtra apenas os arquivos que indicam um arduino

	if(dif.size() == 0)
	{
		cout << "ERRO: Nenhum dispositivo novo detectado" << endl;
		return std::string("");
	}
	else if(dif.size() > 1)
	{
		cout << "ERRO: Mais de um arduino novo detectado" << endl;
		return std::string("");
	}

	return devDir + dif[0];
}