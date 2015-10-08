//////////////////////////////////////////////////////////////////////////////////
//
// Esse codigo é um exemplo de como ler um vetor de tamanho variável publicado
// pelo pacote quare_detect.
//
// square_detect publica menssagens do tipo squareCenters que é definido no
// diretorio msg do pacote.
// Essa menssagem customizada contém apenas um campo chamado centers e
// que é um vetor de objetos do tipo Point (ver callback)
//
// Esse programa lê as menssagens e informa
// no terminal o conteudo do vetor centers

#include <ros/ros.h>
#include <geometry_msgs/Point.h>


#include "nome_do_pacote/squareCenters.h" //custom message

#include <iostream>

using namespace std;

void callback(const teste::squareCenters::ConstPtr& msg)
{
	//a menssagem contem apenas um campo que se chama centers e é do tipo vector
	//ao acessar msg->centers, pode-se tratar centers como se fosse um
	//vector declarado com a sintaxe:
	// 
	//std::vector < geometry_msgs::Point > centers;	

	system("clear");
	ROS_INFO("Got array of size %i", int(msg->centers.size()));
	ROS_INFO("Content:");
	for(int i = 0; i < msg->centers.size(); i++)
	{
		ROS_INFO("point %i", i);
		ROS_INFO("\tx: %i,y: %i", int(msg->centers[i].x), int(msg->centers[i].y));
	}
	ROS_INFO(" ");
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "teste_node");
	ros::NodeHandle nh;
	
	ros::Subscriber sub = nh.subscribe("/visao/squares", 10, callback);

	ros::spin();

	return 0;
}

