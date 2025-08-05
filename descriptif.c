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

// fonctionnalite obligatoire.
// nickname username join msgprv

// client msg channel pour tout les client ayant rejoins ce channel

// on doit avoir des operator et des utilisateur basiques

// les cmd specifique au operateur de canaux

// KICK INVITE TOPIC MODE

// -t -k -o -l

// erreur a gerer dans le chapitre exemple de test.

// ctrl + D pour les commandes partiellement recu, concatener les paquet recu afin d'avoir la cmd entiere


