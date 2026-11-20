# MP3 Player (C / CSFML)

Lecteur MP3 personnel qui sert aussi de **carnet de statistiques** : temps d'écoute réel,
artistes, playlists, et classements Elo par « battles » entre deux musiques ou deux artistes
(inspiré d'une appli Reddit de classement : tier list + battle + activité).

À l'origine prévu pour tourner sur un **Raspberry Pi 4** : ce n'est plus d'actualité, mais
ça pourrait revenir. Garder le programme léger (CPU, mémoire, taille des assets).

## Règles de travail

### Langage
- **C + CSFML 2.6 uniquement** (installée dans `/usr/local`). Ne jamais proposer de
  changer de langage ou de bibliothèque graphique.
- Garder un niveau de code cohérent avec celui de l'auteur (étudiant Epitech) : pas
  d'astuces obscures, préférer le simple et lisible.
- Répondre en français.

### Norme Epitech
Document : `/home/ceh0s/Documents/epitech_c_coding_style.pdf`.
Le vérificateur officiel est le compilateur **epiclang**, utilisé par `make debug` : les
violations s'affichent comme des warnings sur stderr. Objectif : **zéro remarque**.
Ne plus utiliser coding-style-checker.

Rappels principaux :
- 20 lignes max par fonction, 80 colonnes, 4 paramètres max ;
- 10 fonctions max par fichier, dont 5 non `static` max ;
- pas d'imbrication de conditions de profondeur 3 ou plus ;
- pas de variable globale non `const` ;
- structures passées par pointeur, déclarations en début de fonction ;
- continuation d'une condition de `if` : un niveau d'indentation de plus que le corps ;
- pas de macro sur plusieurs lignes (découper en plusieurs macros d'une ligne).

**Pourquoi cette norme** : pour garantir une utilisation efficace des ressources du PC,
mais aussi et surtout pour garder une **lisibilité et une organisation propres**. L'idée de
la norme est de forcer à construire le projet de manière **structurée et logique**, et c'est
ce qui est attendu ici aussi :
- **Fractionner** les gros fichiers en fichiers plus petits et plus explicites **avant**
  d'arriver à la limite de fonctions, plutôt que de tasser du code pour rester en dessous.
- Utiliser des **sous-dossiers** dans le même but (comme dans wolf3d :
  `src/init`, `src/update`, `src/destroy`...).
- Trouver l'équilibre entre trop compact et trop étalé : **chaque fichier remplit sa
  propre tâche**, et son nom dit laquelle.
- Chaque fichier commence par l'en-tête Epitech avec une description utile.

**Exception** : la norme peut être enfreinte **uniquement** si une fonctionnalité l'exige
**et** qu'aucune alternative de taille raisonnable et de coût système non excessif n'existe.
Toujours préférer l'alternative conforme quand elle existe. Chaque écart doit être signalé à
l'auteur et commenté dans le code.
Exemple : Ctrl+C est géré sans variable globale, en bloquant les signaux avec
`sigprocmask` et en les relevant à chaque tick avec `sigpending` (`src/core/signals.c`).

**Dépendance externe acceptée** : le téléchargement lance le programme `yt-dlp` (avec
`ffmpeg`), installé à part (`sudo pacman -S yt-dlp`). Ce n'est pas une bibliothèque liée :
sans lui, l'appli fonctionne et l'écran de téléchargement l'indique.

### Avant de rendre la main
1. `make re` sans warning (`-Wall -Wextra`).
2. `make debug` sans aucune remarque epiclang (puis `make fclean`).
3. `valgrind --leak-check=full ./MP3player` : 0 erreur, 0 fuite.
4. Ne jamais commiter sans accord. `music/` et `data/` sont ignorés par git.

## Architecture

```
main.c
include/   app.h (app_t)  scene.h (scènes, home_t, libscene_t, edit_t, battle_t, ranking_t,
           pledit_t, boards_t)  song.h (song_t, fprint_t, artist_t, link_t, playlist_t,
           board_t, rule_t, library_t, id3_tag_t)  elo.h (arena_t, elo_t, rank_t, pool_t)
           player.h  ui.h (ui_def_t, ui_t, widgets_t, image_t)  mp3player.h (constantes,
           prototypes)
src/core/              création/destruction de l'app, boucle à tick, fenêtre, vue, signaux
src/scene/             table des scènes
    home/              écran d'accueil (bouton play + menu Bibliothèque / Battle / Classements)
    library/           liste des musiques + barre de lecture + onglets de playlists
                       (library_scene.c, _list, _view, _tabs, _bar, _layout)
    edit/              écran d'édition (page principale, lignes d'artistes, recherche/fusion,
                       saisie, suggestions, page artiste)
    battle/            face-à-face de deux musiques ou deux artistes (scène, vue, vote)
    ranking/           tier list (noms ou images : ranking_tiles.c) et leaderboard
    playlist/          édition d'une playlist (nom, titres cochés, suppression)
    boards/            gestion des classements personnalisés (liste, règles, ajout)
    download/          écran de téléchargement (adresse, progression, annulation)
    pages/             morceaux d'écran réutilisables (artist_page, category_bar,
                       board_bar : classement + musiques/artistes, image_box : import)
src/ui/                tables d'éléments (atlas), collisions, formes, widgets,
                       widgets image / infobulle, cache des textures d'images
src/elo/               catégories, calcul Elo, score (TOTAL), pool d'une arène, paires
src/audio/             lecteur (un seul sfMusic ouvert), file de lecture, temps d'écoute
src/download/          téléchargement audio par yt-dlp (lancement, suivi à chaque tick)
src/library/           chargement / rafraîchissement / destruction de la bibliothèque
    file/              fichiers sur disque : validation mp3, empreinte audio, scan du dossier
    id3/               lecture des tags ID3 et de la pochette (APIC)
    images/            images des musiques/artistes : miniatures, import, nettoyage du chemin
    playlists/         playlists et vue des musiques présentes d'une playlist
    boards/            classements, règles inclure/exclure, appartenance d'une musique
    songs/             chansons, empreintes connues, résolution des alias, édition, fusion
    artists/           artistes, liens, noms affichés, stats et détail par artiste
src/data/              lecture/écriture TSV, migration, journaux
src/utils/             UTF-8, chaînes, saisie de texte (avec Ctrl+V)
assets/       atlas.png (6 icônes 256 px), fonds JPEG, font.ttf, no_cover.png (image par
              défaut, provisoire : l'auteur fournira la sienne)
```

Principes en place :
- **Boucle à tick** (reprise de wolf3d) : 33 ms par tour, `sfSleep` du reste.
- **Vue étendue** : coordonnées de design 1920×1080, la vue garde au moins cette zone et
  s'agrandit selon le ratio (1920×1200 en 16:10, plus large en 21:9), sans déformation.
  Souris convertie avec `sfRenderWindow_mapPixelToCoords`.
- **Tables `const` d'éléments** (idée de myworld) : `{id, anchor, {x, y, w, h, tx, ty, tw, th}}`,
  ancrage `ANCHOR_TOP` / `ANCHOR_CENTER` / `ANCHOR_BOTTOM`. La même table sert à l'affichage
  et aux clics.
- **Table de scènes** (comme `st_update` de wolf3d) : `init / event / update / draw / layout /
  destroy`, `update` peut être `NULL`. Seule la scène affichée est chargée.
- **Navigation comme un chemin de dossier** (`src/scene/scene_nav.c`) : `app->path` garde les
  scènes parentes depuis l'accueil. `scene_open` descend (`cd battle`), `scene_back` remonte
  (`cd ..`) et ferme l'appli depuis l'accueil. Échap appelle `scene_back` (dans `global_event`,
  sauf l'édition et la gestion des classements, qui remontent d'abord leurs sous-pages ;
  Espace = lecture/pause sauf dans les écrans de saisie). Ne jamais écrire `next_scene`
  directement.
- **Widgets en mode immédiat** (écran d'édition) : une fonction décrit l'écran, appelée en
  `WG_DRAW` pour dessiner et en `WG_CLICK` au clic ; un seul `sfText`, un seul rectangle,
  une forme image carrée et une ronde réutilisés. L'infobulle (`widget_tip`) est mémorisée
  puis dessinée en dernier (`widget_tip_draw`).
- **Images** : chargées à la demande (`image_get`) et libérées à chaque changement de scène.
  Musiques carrées, artistes ronds (comme YT Music).
- **Téléchargement** : `fork` + `execvp("yt-dlp")` dans son propre groupe de processus,
  sortie lue sans attendre (tube `O_NONBLOCK`) et fin détectée par `waitpid(WNOHANG)` à
  chaque tick : ni thread ni variable globale. À la fin, `library_rescan` ajoute le mp3
  (titre, artiste et pochette viennent des tags écrits par yt-dlp). Annulé à la fermeture.
- **Arène** (`app->arena`) : type (musiques/artistes) + classement + catégorie, partagée par
  les scènes battle et classements.

## Données (`data/`, format TSV v2)

Les fichiers mp3 ne sont **jamais modifiés** ; tout ce qui est édité vit dans `data/`.
Rien n'est jamais supprimé : une musique retirée garde ses stats (`present = 0`).

| Fichier | Contenu |
|---|---|
| `songs.tsv` | une ligne par musique : id, merged_into, titre édité, tags ID3 d'origine, compteurs |
| `fingerprints.tsv` | empreinte audio d'un fichier → id de musique |
| `artists.tsv` / `song_artists.tsv` | artistes, et liens musique ↔ artiste avec rôle (main/feat/remix) |
| `listens.log` | une ligne par écoute : date, id, ms réellement écoutées, fin (complete/skip/quit) |
| `edits.log`, `merges.log` | historique des modifications et fusions/séparations |
| `battles.log` | une ligne par battle : date, type (song/artist), classement, catégorie, gagnant, perdant (ids bruts), Elo d'avant |
| `elo.tsv` | copie lisible des Elo calculés (jamais relue) |
| `playlists.tsv` / `playlist_songs.tsv` | playlists, et leurs musiques dans l'ordre (ids bruts) |
| `boards.tsv` / `board_rules.tsv` | classements personnalisés et leurs règles (include/exclude, all/playlist/artist/song) |
| `images/songs/`, `images/artists/` | `<id>.jpg` (400 px) et `<id>_s.jpg` (120 px), carrés |

- **Identité** : id interne fixe + empreinte audio (FNV-1a des 256 premiers Ko après le tag
  ID3, plus la taille audio). Un fichier renommé ou re-tagué reste reconnu.
- **Fusion** : la source devient un alias (`merged_into`) ; rien n'est recopié, les stats
  sont additionnées à l'affichage (`song_total_listen`), donc « Séparer » annule exactement.
- **Stats par artiste** : calculées à partir des liens actuels, jamais stockées. Changer
  l'artiste d'une musique déplace tout son historique. Détail (`artist_stats`) : total de
  toutes les musiques où il apparaît, dont original / auteur d'un titre remixé par un autre /
  remixeur / featuring. Affiché par la page artiste (clic sur un artiste dans l'édition).
- **Elo** : `battles.log` est la source de vérité, rejoué au chargement et après chaque
  fusion/séparation avec les ids résolus. Une fusion regroupe donc les Elo et « Séparer »
  les restaure exactement ; une battle entre deux musiques fusionnées depuis est ignorée,
  comme une catégorie retirée du tableau ou un classement supprimé (la ligne reste dans le
  log, l'id d'un classement supprimé n'est jamais redonné). Catégories enregistrées par
  leur nom. Le premier `battles.log` (sans type ni classement) a été archivé en
  `battles.log.v1.bak` : reset de tous les Elo le 2026-11-20.
- **Classements** : chacun a ses propres Elo. Le « Général » (id 0) contient toutes les
  musiques présentes, n'est ni enregistré ni modifiable. Une musique est dans un classement
  si une règle l'inclut et aucune ne l'exclut (règle artiste = tous rôles). Les artistes
  d'un classement sont ceux liés à ses musiques.
- **Playlists** : ids bruts résolus à l'usage ; « Tous les titres » = musiques présentes.
  Le lecteur joue dans la file de la playlist choisie (`player_queue`). Supprimer une
  playlist retire les règles de classement qui l'utilisent.
- **Images** : la pochette ID3 est extraite une fois au chargement si l'image n'existe pas ;
  remplaçable depuis l'édition (chemin collé) ou reprise du mp3. Image d'une fusion = celle
  de la musique résolue.
- Écriture dans `<fichier>.tmp` puis `rename()` (pas de corruption en cas de crash).
- Sauvegarde : au lancement, toutes les 30 s, à chaque changement de musique, à la fermeture
  (y compris Ctrl+C).
- L'ancien format v1 est converti automatiquement (originaux gardés en `*.v1.bak`).

## Plan de développement

### Phase 4.5 — Playlists, classements personnalisés, battles d'artistes, images (faite)
- Catégories : **Tout confondu** (jugement libre, indépendant, catégorie principale, jamais
  tirée au hasard, exclue de TOTAL), Mélodie, Production, Émotion, Rejouabilité,
  Nostalgie. Mêmes catégories pour les artistes.

### Phase 4 — Battle, Elo, tier list, leaderboard (faite)
- **Elo standard** : `E_a = 1 / (1 + 10^((R_b - R_a) / 400))`,
  `R_a' = R_a + K * (S_a - E_a)`, départ 1500, K = 40 pour les 10 premières battles d'un
  titre puis 20.
- **Classements** : un Elo **Tout confondu** indépendant (ex-GLOBAL), un Elo **par
  catégorie** (liste dans un tableau `const` facile à étendre), et une catégorie virtuelle
  **TOTAL** = somme des Elo de catégorie (calculée, pas stockée).
- **Scène BATTLE** : deux cartes (titre, artistes, Elo ; pas d'extrait, décision de l'auteur),
  choix de la catégorie + bouton **« catégorie aléatoire »** (jamais Tout confondu), bouton passer,
  flèches gauche/droite pour voter. K compté par musique et par catégorie. Paires tirées en
  privilégiant les titres avec peu de battles, puis un adversaire parmi les 3 Elo les plus
  proches, parmi les musiques présentes.
- **Tier list** S/A/B/C/D par percentile (10 / 25 / 30 / 25 / 10 %) et **leaderboard**
  (Elo, battles, % de victoires, défilement). Seules les musiques présentes avec au moins
  une battle dans la catégorie sont classées. Accès depuis l'accueil et la bibliothèque.
- Toujours utiliser l'id **résolu** (`song_resolve`) : une fusion doit regrouper les Elo.
- Écrire `elo.tsv` (`song_id category rating battles wins`) et `battles.log` (rejouable).

### Phase 5 — Écran de statistiques
Temps d'écoute réel (total / 7 j / 30 j depuis `listens.log`), top artistes (co-artistes
comptés, avec le détail de `artist_page`), albums, genres, taux de skip, historique. L'auteur ajoutera ses propres idées.

### Idées notées pour plus tard
- Reconnaissance audio (AcoustID / MusicBrainz) pour identifier automatiquement artiste et
  titre : demande Internet et des bibliothèques en plus.
- Saisie de texte : curseur déplaçable, texte trop long qui dépasse du champ.
- Onglets de playlists : défilement quand il y en a trop (ceux qui dépassent sont cachés).
- Page artiste accessible ailleurs que depuis l'édition d'une musique (pour l'image).
- Fusion d'artistes (les Elo d'artistes ne sont pas encore résolus).
- Retour éventuel sur Raspberry Pi 4.

## Références
- Boucle à tick : `/home/ceh0s/EPITECH/tek1/G-ING/wolf3d/src/update/update_events.c`
- Placement par tableau de floats + atlas : `/home/ceh0s/EPITECH/tek1/G-ING/myworld/src/interface.c`
