# OPEN 2015

Esse branch do repositório regista o código que foi usado no open 2015 pela equipe Autobotz.

A maior parte do código é compatível com o ROS e portanto deve ser compilada com o utilitário catkin_make a versão do ROS utilizada foi a indigo.

O script rebuild\_ws.sh na raiz do repositorio limpa as pastas que nao sao de codigo fonte e faz catkin_make duas vezes. Seu uso após baixar ou atualizar o repositório evita alguns problemas comuns.

'autobotz\_ws/config' contem arquivos de configuração para o código de visão e para o robo.
'autobotz\_ws/src/barco\_launch' tenta automatizar o processo de iniciar todos os programas do barco
Os códigos de arduinos estão em 'autobotz\_ws/src/eletronica/codigos\_arduino'

# Compilando e rodando

O workspace é compilado rodando 'catkin\_make' no diretório 'autobotz\_ws'. Após compilar pela primeira vez é preciso dar 'source devel/setup.bash'.
Nota: Esse comando _pode_ ser adicionado ao seu '~/.bashrc'.


# Problemas Comuns

Os códigos usam openCV e rosserial\_arduino, certifique-se de que essas bibliotecas estão instaladas

Se o rosrun não consegue encontrar seus pacotes, certifique-se de que deu 'source devel/setup.bash'.

Os códigos de Arduino precisam que as bibliotecas do rosserial sejam adicionadas à pasta de bibliotecas da IDE. Consulte a documentação do rosserial\_arduino para detalhes.
