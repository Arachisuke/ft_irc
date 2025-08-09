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