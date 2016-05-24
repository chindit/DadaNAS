# GUIDE D'INSTALLATION

## Prérequis
* Un PC connecté au réseau local (NAS) fonctionnant avec un système UNIX
* Une connexion à Internet fonctionnelle
* SSH, Cron et serveur Web installés sur le NAS
* WakeOnLan (paquet «wol») installé sur le client s'il s'agit d'un UNIX.  Rien à faire s'il s'agit de Windows.
* Le PC client *doit* fonctionner sous UNIX ou Windows sinon DadaNAS ne fonctionnera pas

## Installation
### Sur le serveur
* Placer le fichier «nas.php» à la racine du serveur web.  Il doit être accessible via «http://mon.ip.locale/nas.php»
* Créer un dossier «tmp/» à la racine du serveur web et donner les droits d'écriture dessus (`chown http tmp/` par exemple)
* Éditer le fichier «autosuspend.sh» et remplacer «IP_LOCALE:PORT» par l'IP de votre NAS (Ex: 192.168.1.15 et le *port SSH* [généralement le 22])
* Donner les droits d'exécution (`sudo chmod +x autosuspend.sh`) et le placer dans `usr/bin/`
* Configurer Cron pour qu'il exécute à intervalle régulier ce script.  Personnellement, je le lance toutes les 10 minutes:
  * `*/10 * * * * /usr/bin/autosuspend.sh`

### Sur la/les machine(s) locale(s)
* Compiler DadaNAS
  * `qmake && make`
* Lancer DadaNAS et remplir les informations dans «Outils/Options»
* *Attention!* Par défaut, le gestionnaire de fichiers est _dolphin_  Pensez à le changer si vous le souhaitez, mais votre gestionnaire de fichiers doit accepter la commande `gestionnaire smb://ip.locale.du.nas:/partage`

C'est tout!
DadaNAS fonctionne!
Bonne utilisation ;)
