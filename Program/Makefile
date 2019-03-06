DIR = /etc/systemd/system
DIR_CONF = /etc/webserver
DIR_SERVER = ./WebServerDir/
DIRH = ./include
SERVICE = WebServer.service
CONF_FILE = config.conf
SERVER = WebServer
SRC = main.c Common.c
BIN = main


all: $(BIN)
		sudo  mkdir -p $(DIR_CONF)
		sudo cp $(CONF_FILE) $(DIR_CONF)
		sudo cp $(SERVICE) $(DIR)
		sudo systemctl daemon-reload
		sudo systemctl enable $(SERVER)
		sudo systemctl start $(SERVER)
$(BIN): 
		gcc $(SRC) -I$(DIRH) -o $(DIR_SERVER)WebServer
