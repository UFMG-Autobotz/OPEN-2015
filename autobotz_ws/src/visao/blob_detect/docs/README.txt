Pacote de visão para o barco a ser usado no OPEN 2015
Desenvolvido por Pedro Blanc Árabe, equipe Autobotz/UFMG

Esse documento apresenta algumas dicas.

SOBRE O USO:
	Esse pacote contém o nó blob_detect. Para rodar:
		roscore
		roslaunch freenect_launch freenect.launch
		rosrun blob_detect blob_detect

	Vários parâmetros da visão computacional podem ser mudados após a compilação através do arquivo
blob_detect.conf
	O pipeline de visão está explicado mais detalhadamente no código, mas a alto nível pode-se
dizer que ocorrem os seguintes passos:
		imagem -> detecção de bordas -> identificação de regiões -> encontrar cor média de cada região -> nomear cor de uma região de acordo com uma paleta
	
	Uma paleta é um conjunto de cores. Nesse programa, considera-se ainda que cada a cada cor
estão associados um nome e uma distância máxima. O programa define uma métrica de distância entre cores 
na função colorDistance(). No último passo, uma cor que está a uma distância menor do que 'distância máxima'
de determinada cor na paleta é associada àquela cor.
	A paleta usada nesse último passo é definida no arquivo palette.conf (ver FORMATOS DE ARQUIVOS abaixo)
	
	O conjunto de uma região de interesse na figura e os dados associados à região (ex: cor média, área)
é chamado de 'feature'. O programa publica para o ROS um vetor de features.

TÓPICOS:
São usados um tópico de tipo sensor_msgs/Image como entrada do vídeo e um
tópico de tipo featureVec como saída.
Os nomes de tópicos são definidos em arquivo de configuração.

FORMATOS DE ARQUIVOS:
Arquivo de paleta:
	o arquivo padrão é ~/open-2015/autobotz_ws/config/visao.conf
	Cada linha tem o formato:
	"Nome_da_cor B G R T"
	Onde Nome_da_cor é uma string que identifica aquela cor. Esse campo é obrigatório. Cores podem ter nomes repetidos.
	R G e B são números de 0 a 255 que dão a intensidade de cada canal da cor.
	T define uma região em torno do valor (B,G,R) que é considerada pertecente àquela cor.
	A atual métrica de distância é a norma-L2, então T estabelece uma esfera de raio T em torno do ponto (B,G,R)

Arquivo de configuração:          <-------------------------- A ser implementado!!
	o arquivo padrão é ~/open-2015/autobotz_ws/config/visao.conf
	Cada linha define um parâmetro e tem a sintaxe:
	"TTT Nome Valor"
	Onde TTT são três letras que denominam o tipo de dado que a linha define.
	Nome é o nôme do parâmetro sendo setado.
	Valor pode ser um número ou uma string.
	Somente valores já esperados pelo programa são reconhecidos. Espaços extras são ignorados.

FORMATOS DE MENSSAGEM:
	O programa publica menssagens do tipo definido no arquivo features.msg
