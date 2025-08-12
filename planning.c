// FINIR la fonction lecture @@@@@
// faire la fonction envoie.@@@@@


// gerer POLL.
// rendre non bloquant avec fcntl.
// lire select
// plusieurs client.

// commande bloquante
// signaux, commande partiel.
// les commentaire a affiche a l'entrer du server
//



// ANDY
// vu que la connexion est etabli, entre le serveur et le client
// je lis les entree avec la fonction lecture qui renvoie entry, une std::string
// avec entry -> comparer avec les cmd qu'on doit implementer et ensuite les implementer.
// dans les cmd a implementer c'est ecris dnas le sujet, et j'ai mis bcp bcp d'info sur les cmd dans descriptif si t'arrives a lire, et faut surtout ce referer aux lien 2, de thib, et au premier de felipe.
// les commmandes ont des regles vraiment specifique, chaque cmd a des messages et des erreurs .




// epoll _ fd -> epol create = cree un tableau virtuel, et y ajoute epoll_fd pour pouvoir y acceder par la suite
// epoll_ctl - >surveille ce fd. ex : epollin par ex lecture dispo
// 

nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

for i = 0 < nfds

if (event[i].data.fd == listen_fd)
// nfds = nombre de reaction
        // event de epoll wait, c'est un tableau imaginaire ou imaginons la reactions 1 = fdserveur alors ca veut dire que quelquun tente de parler a mon serveur
        // ensuite si ca correspond a un autre fd, j'ecoute simplement ce fd.
        // nfds = epoll wait.. attend qu'un client lui parle // et tu le met dans ta liste de client.
        // ensuite tu boucles sur tout ces fds qui ont reagis pour les faire parler.
        // si dans le tableau events(ou ya les fd qui ont reagis) y en a un qui est egal au fd client 
        // ca veut dire que quelquun veut parler a notre fd, du coup je l'accept je prend son fd je le met dans ma liste de clientfd a surveiller pour les prohaines actions et la j'ecoute avec recv
        // si non j'ecoute car c'est un client deja connecte, si la len est 0 le client s'est deco on close et le retire dans notre liste de surveillance.
        
// epoll fd -> tout les fd serveur client
// events -> tout les fd qui ont eu un event.




// etapes 
// ne plus close l'hote des que ya un erreur maintenant qu'il est multiclient . @@@@@
// cree une fonction registration ? -> dans la case newclient de wait client je lui demande tout ce qui est requis pour etre accepte dans mon server et apres je lui envoie rpl_welcome. @@@@@@
// class client -> nom, prenom, fd, channel actuelle, role actuelle. avec sa struct sockaddr, et son socklen. et ++++ @@@@
// finir la fonction server avec les modifs de class, @@@@
// finir de faire la fonction PUSHMSG -> booleans, si RPLWELCOME vaut 0 -> push RPLWELCOME. les autres se sera des consequences de cmd du coup c la cmd elle meme qui renvera le bon msg @@@@@
 // ensuite verifier si EPOLL marche. @@@@@


// faire les commandes partiels.
// comment une discussion entre un server/client se passe ? le serveur lui parle le client repond ? ou il peut y avoir des monologues ? c'est pour gerer le epollin et epollout, est ce que une fois que j'ai envoye un msg je le repasse en epollin pour ecouter sa reponse comme a linverse sil menvoie un msg je le met en epollout afin de traiter sa demande.


        
// est ce que je dois arreter le server si ya une erreur ? dans un client ou un fd negatif etc.


        
// enlever le prompt, de server il sers a rien, et enlever le prompt de client quand ya une erreur, et mettre un msg d'erreur
// faire les signaux
// verifier mes messages envoyer et recu et leur normes.