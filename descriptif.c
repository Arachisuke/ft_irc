// pas de connexion server a server
// pas a developpe de client.

// /ircserv <port> <password>
// port du serveur
// mot de passe identifiant le client au serrver
// poll epoll ou kqueue fonction

// pouvoir gerer plusieurs client sans boque
// le forking est interdit toute les operation entre/sortie doivent etre non bloquante

// droit qu'a un seul poll ou equivalent

// obliger d'utiliser poll afin de ne pas trop utilise de ressource si j'utilise read/recv ou write/send
// il existe plusieurs client irc on doit choisir l'un deux.

// le client de ref doit pouvoir se connecte a notre serveur sans erreur

// le protocole qu'on doit utilise c'est TCP/IP

// fonctionnalite obligatoire !!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// nickname username join msgprv

// client msg channel pour tout les client ayant rejoins ce channel

// on doit avoir des operator et des utilisateur basiques

// les cmd specifique au operateur de canaux

// KICK INVITE TOPIC

// MODE -i -t -k -o -l -> les modes du canals different de channel ?

// erreur a gerer dans le chapitre exemple de test.

// ctrl + D pour les commandes partiellement recu,
	concatener les paquet recu afin d'avoir la cmd entiere

// ./irc "port" "password"
// servers policy restrictions,
	length (63) et ce qu'il peut etre utilise ou non dans un nom de server
// TCP 6667/6697 ? password c'est pour entrer dans le server ? qui le cree ?

// REGISTRATION
// PASS ? password connection,
	il est deja dans nos param de irc Numeric Replies: ERR_NEEDMOREPARAMS  ERR_ALREADYREGISTRED

/* enter NICK <nickname> and USER <username> <mode>
	-> rajouter policy restrictions on nickname length etc
 NICKNAME are non empty strings with following restrictions:
 ......length (9) */
// MSG registration finish
// RPL WELCOME
	// RPL YOURHOST lui c'est pour les services a priori.// RPL CREATED
	// RPL MYINFO // RPL_ISUPPORT
// client-to-server protocol structure la discussion entre un client et un serveur,
	c'est un stream of bytes, this streams contains message sperated by cr
	/r and /n
// channels -> names are strings begin with & #regular + ! (50)
// poll -> event ??
// JOIN <channel>,<channel> <key>,<key>
// key -> ?

// check if the client can join the channel and after that procedding.
// INFO channel msg -> JOIN,PART,KICK,MODE,TOPIC, PRIVMSG,NOTICE,
	et receive QUIT msg from other client joined the same channel.
// info msg policy, <source>nameofthechannelasthefirstparameterofthemessage

// the RPL_TOPIC, and no msg if no topic.
// a list of users currently joined to the channel.

// KICK -> <channel> <user> *( "," <user> ) [<comment>]
// kick msg -> <source = client qui a kick> <channel d'ou il a ete kicker>
// if no comment is given by the client who kick,
	default msg instead le default c'est juste command to kick name from channel sinon tu rajoute using : comment as the reason
// pas de multicomment -> un comment par kick.
// limit de target par kick,
	si ya trop de target tu dois silencieusement stop la cmd.

// INVITE -> <nickname> <channel>
// the channel should exist sinon
	-> ERR_NOSUCHCHANNEL. seulement ceux qui sont deja dans le channel qui peuvnet inviter SINON ERR_NOTONCHANNEL
// if invite only, et que t'es pas un operator tu a l'erreur chanoprivneeded.
// s'il est deja dedans erreur il est deja la.
// if sucessful, the serveur must send RPL_INVITING, to the command issuer,
	invite message with the issuer as <source> <channel> to the target user.

// TOPIC ->  <channel> [<topic>]
// used to change or view the topic of the given channel.
// si jenvoie pas de topic, jenvoie RPL_TOPIC, en montrant le topic du channel
// si topic cest une chaine vide, le topic est clear.
// si t;essaye de voir el topic alors que t'est pas de dedans, err_notonchannel

// MSGPRV -> <target>{,<target>} <text to be sent>
// target - nickname ou channel
// if mode qui bloque le msg ou si le user est ban, silently fail.
// si dans tout ces cas precise ou il peut pas envouye de msg tu envoie le msg
	-> ERR_cannotsendtochan.
// au user qui envoie le msg.

// ctrl +d -> pour les commandes partiels.
// gerer les signaux pour bien commence le projet.

// socket -> reecrire tout ce que t'as lu de la doc.
// ecrire socket en detail et commencer la machine.

// faire la socket, c'est la commmunication client - server.

// TCP/IP -> sys/socket.h
// creation de socket et de son fd par socket()
// bind -> permet de speciifer le tupe de com associete au socket tcp ou UDP.
// socket -> bind -> listen() -> accept() Read() write() close()

// famille : represente la fa,ille de protocole AF_INET une addresse internet sur 4 octets,
	l'adresse ip ainsi qu'un numero de port.
// TYPE : SOCK_STREAM.
// protocole -> 0 pour TCP.
// verifier le -1 du FD.
// bind -> fd,
// TCP AF_INET -> sockaddr_in netinet/in.h

// short 2 octet, sin_family -> AF_INET
// sin port -> htons - 6667 big endian
// sin_addr -> htonl - ANNYDDR.
// sinzero -> mettre des zero. bzero.

// OPER <name> <password>
// regular channel #, local channel & -> first joined, is the operator.
// MODE -> sujet et doc different.

/* 🧠 Analogie :
Imagine une maison (IP) avec plusieurs pièces (ports).

L’IP te donne l'adresse de la maison.

Le port te dit à quelle porte frapper pour parler au bon service.

Tu peux avoir plusieurs serveurs différents sur une même machine,
	chacun écoutant sur un port différent, mais avec la même IP. */

// port en gros la piece dans laquelle le client doit frappe il connait la maison via l'Ip manque la piece "l'endroit ou dort lapplication"

// recv verifier le 512 bytes. // et aussi le no signal.
// faire les signaux , ctrl C eT D.