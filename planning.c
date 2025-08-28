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


// comment une discussion entre un server/client se passe ? le serveur lui parle le client repond ? ou il peut y avoir des monologues ? c'est pour gerer le epollin et epollout, est ce que une fois que j'ai envoye un msg je le repasse en epollin pour ecouter sa reponse comme a linverse sil menvoie un msg je le met en epollout afin de traiter sa demande.


        
// est ce que je dois arreter le server si ya une erreur ? dans un client ou un fd negatif etc.


        
// enlever le prompt, de server il sers a rien, et enlever le prompt de client quand ya une erreur, et mettre un msg d'erreur
// faire les signaux


// registration au propre.
// je l'enregistre en client et le met en epollin.
        // il est en epollin il a reagis, je lui demande le mdp, je le remet en epollin.
        // je lui demande le nickname, je le remet en epollin.
        // je lui demande le username, je le met en epollout pour lui envoyer le msg de bienvenue.
        // je le remet en epollin pour qu'il puisse faire autre chose @@@@
// faire la fonction Le big 3, Close, Delete, et Erase. + MSG client disconnected @@@@
        // rajouter le try and catch@@@@
        // atoi supprimer ou faire ton atoi @@@@

        // est ce a jour ? @@@@

        
        
        
        
        // le big3, possiblement mal implemente car c'est une methode interne dangereux si quelqu'un l'appel. @@@@@
        // verifier mes messages envoyer et recu et leur normes.@@@@@@
        // ne pas oublier les signaux.
        // faire les commandes partiels.
        // refaire proprement le BIG3, et les messages d'erreurs. -> ou close le fd etc ...
        // INIT LES FD a -1 !!!!! @@@@
        // commencer les commandes -> JOIN etc.
        // faire la class CHANNEL.
        // if JOIN envoyez a la place de PASS, envoyer msg d'erreur t'es pas connecte.
        // relire la DOC.
        
        // parse les commandes. -> SI pas connecte (bool connecte) renvoyez l'erreur correspondante a ce qu'on m'as envoye.
        // 



          // parse les commandes. -> SI pas connecte (bool connecte) renvoyez l'erreur correspondante a ce qu'on m'as envoye.
        
        // je dois envoyer la cmd + son param. et s'il se trompe envoye le man de la cmd ?
        
        // cmd partiel


// Parse_cmd -> if pas connecte -> msg d'erreur.
// Parse_cmd -> if connecte -> cmd -> si cmd existe -> cmd(param) -> si param existe -> 


        // theorie du parsing
        // rajouter le booleen connecte ou non.
        // si pas connecte -> msg d'erreur correspondant a la cmd envoyer.
        // verifie son prototype sur la doc.
        // si connecte -> cmd(param) -> si cmd existe -> cmd(param) -> si param existe -> si tout est bon on lance la cmd.


        // theorie de la cmd partiel.
        // tant que t'as pas recu /r/n continue de lire.
        // si t'as recu /r/n -> parse la cmd.
        // si la cmd est bonne -> execute la cmd.
        // si la cmd est mauvaise -> msg d'erreur.
        // si la cmd est partiel -> continue de lire.
        // si la cmd est complete -> execute la cmd.
        // si la cmd est complete -> si la cmd est bonne -> execute la cmd.
        // si la cmd est complete -> si la cmd est mauvaise -> msg d'erreur.


        // lire tant que elle est pas fini
        // changer la fonction translation to client comme ca elle stock la cmd tant qu'elle est pas fini.
        // une fois fini -> elle passe au parse. (verifie si la cmd est bonne, si elle est bonne verifie si je suis co)
                        

        


        // commande partiel @@@@@
        // signaux @@@@@
        // parse du msg et rediriger selon la cmd envoyer si la cmd c'est pass, bah du coup va dans registration sinon si c une autre cmd et que je suis aps co msg erreur
        /*
        etape du parsing, verifie la cmd avec un find + substr check la liste de cmd de std::Map -> if not exist -> msg d'erreur
        if exist -> check le param -> if not exist -> msg d'erreur
        type de msg -> de la commande chaque commande a des messages d'erreur et de reponses au cmd si elle sont bonne.
        aussi -> il y a une policy restriction sur les cmd, les param etc.
        exemple de cmd -> NICK <nickname> -> policy restriction -> nickname -> length (9) -> si le nickname est trop long -> msg d'erreur
        // gerer la police des arguments tel que nick user et real name, voir combien a de param chaque cmd. stocker dans une struct ou une map ah je peux mettre une struct en valiuer d'une map ? comme ca j'y integre tout les param de la cmd et sa policy restriction. et sa msg d'erreur. et sa msg de reponse. et la fonction a appeler.
        
        
*/
        // PARSE CMD. stdmap cmd = fonction. @@@
        // fonction@@@
        // ignorer la cmd CAP. @@@@@



        // reverifier le big_3, pour finalement dire le big_4, CAR C epolldelete close delete(close fd inside) et erase -> message client disconnected. et erreur @@@@



// define les msg d'erreur. @@@@@
// certain message necessite la <cmd> avant le msg d'erreur. 

// password, c'est que a la fin que je dis que c'est faux. apres USER. @@@@@

// push et readmessage a changer . @@@@@



// decoupe la cmd, find la cmd -> envoie a la fonction execution, ensuite dedans parsing des params, ensuite verifie si je suis connecte, si je suis dans le bon contexte excontext: on me dis exit ou quit alors je suis pas dedans ou je rentre dans un channel alors que je ne peux pas.



// implementer les commandes.
// verifier si dans cmd la chaine est bien decouper en cmd -> arg par node.
// mes message d'erreur je les envoies au server et non pas au client je dois change ca.
// je dois aussi define tout mes message d'erreur
// je dois aussi <client> gerer ca
// je dois aussi gerer le msg special de NICK.
// verifier tout les commentaire de code.


//topic
        // host is registered? continue : error (451)
        // nb params < 3? error : continue (461)
        // channel exist? continue : error (403)
        // nb params == 2? read mode : set mode
                // read mode (tout le monde peut lire)
                        // topic empty? 331 : send topic + send topic setter
                        // return
                // set mode: host present sur le channel? continue : error (442)
                        // topic restricted and host not operator? error : continue (482)
                        // extract new topic cmd[3] et delete ":" du debut
                        // set topic et topic setter
                        // ostream: notifier tout les membres du channel

   
