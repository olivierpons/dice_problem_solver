Trouvez toutes les solutions pour moi s'il vous plaît !
====================================================

J'ai décidé de coder un programme qui recherche toutes les solutions pour ces deux puzzles (cherchez "oldchap.games" sur Google) :
- "Coba", écrit par Antonin Boccara et Michel Verdu, publié par Oldchap
- "Par Odin", écrit par Antonin Boccara et Michel Verdu, publié par Oldchap

J'ai d'abord utilisé l'aide de ChatGPT, mais bien sûr, comme dans tous les cas où il faut faire quelque chose d'avancé, il n'a servi que d'assistant, et j'ai dû finir toutes les parties du code qui étaient vraiment compliquées.

"J'ai terminé l'implémentation complète, et maintenant mon ordinateur tourne pour rechercher toutes les possibilités avec 7 dés blancs du premier puzzle et 7 dés blancs du deuxième puzzle.

Je vais ajouter toutes les solutions qui ont été trouvées en commentaires dans le code. Ensuite, il y a quelque chose de plus complexe, qui est d'ajouter tous les dés noirs et toutes les combinaisons de tous les dés noirs. Pour l'instant, je vais m'arrêter là, et si quelqu'un veut reprendre le code et finir avec toutes les combinaisons de tous les dés noirs, ce serait super de faire évoluer le projet et d'avoir aussi toutes les combinaisons avec tous les dés noirs.

Coba
====
<img src="img-coba-dice.png" alt="Coba" width="300" style="max-width: 300px; height: auto;">

Le moment est venu pour vous d'entreprendre la quête des pierres de Coba... Serez-vous à la hauteur du défi ?

Dans 'Les Pierres de Coba', vous devrez résoudre 50 défis logiques pour restaurer l'équilibre au sein de la société Maya. Dans ce jeu solo, narratif et évolutif, vous découvrirez la richesse mécanique du jeu au travers des différents défis.

Une toute petite boîte pour des heures de jeu !

Par Odin
========
<img src="img-by-odin-dice.png" alt="Par Odin" width="300" style="max-width: 300px; height: auto;">

La légende raconte qu'un jour un humain viendra dans le royaume d'Asgard et jouera aux côtés des dieux... Serez-vous ce héros ?

Dans 'Par Odin', vous incarnez un guerrier humain venu accomplir 50 défis afin de siéger à la table des dieux. Dans ce jeu solo, narratif et évolutif, vous découvrirez progressivement, au fil des défis, la richesse mécanique du jeu et l'univers de la mythologie nordique.

Une toute petite boîte pour des heures de jeu !

## Solutions Trouvées

```
[Captain/0, Peasant/0, Hero/3, Hero/3, Hero/3, Hero/3, Queen/3] = 15 - [Hero/3, Soldier/1, Potter/1, Potter/1, Potter/1, Potter/1, Shaman/7] = 15
[Captain/0, Peasant/0, Queen/3, Queen/3, Queen/3, Queen/3, Queen/3] = 15 - [Hero/3, Hero/3, Hero/3, Hero/3, Soldier/1, Soldier/1, Potter/1] = 15
[Captain/2, Mage/4, Hero/3, Hero/3, Hero/3] = 15 - [Hero/3, Soldier/0, Scribe/0, Scribe/0, Scribe/0, Queen/3, Queen/3, Queen/3, Queen/3] = 15
[Captain/2, Shaman/9, Hero/3, Hero/3, Shaman/9] = 26 - [Hero/3, Hero/3, Hero/3, Soldier/0, Shaman/5, Queen/3, Queen/3, Queen/3, Queen/3] = 26
```

[🌍 Retour à la sélection de langue](README.md)
