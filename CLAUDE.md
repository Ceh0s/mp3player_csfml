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
           pledit_t, boards_t, dlscene_t, dupscene_t, setscene_t, selection_t)  song.h
           (song_t, fprint_t, artist_t, link_t, playlist_t, board_t, rule_t, rejected_t,
           source_t, envelope_t, not_dup_t, dup_info_t, dup_pair_t, like_t, tie_order_t,
           suggest_t, sug_state_t, settings_t, library_t, id3_tag_t)  download.h
           (download_t, dl_options_t, dl_entry_t, dl_report_t)  elo.h (arena_t, elo_t,
           rank_t, ranked_t, pool_t)
           player.h  ui.h (ui_def_t, ui_t, widgets_t, image_t)  mp3player.h (constantes,
           prototypes)
src/core/              création/destruction de l'app, boucle à tick, fenêtre, vue, signaux,
                       touches maintenues (keys.c : seuls Retour arrière et flèches répètent)
src/scene/             table des scènes
    home/              écran d'accueil (bouton play + menu Bibliothèque / Battle /
                       Classements / Doublons / Paramètres)
    library/           liste des musiques + barre de lecture + onglets de playlists
                       + barre de tri et glisser-déposer (library_scene.c, _list,
                       _view, _tabs, _bar, _layout, _sort, _drag), avis (_likes),
                       mode sélection (_select, _select_view) et ses actions en lot
                       (_actions, _action_rows, _bulk, _insert)
    edit/              écran d'édition (page principale, lignes d'artistes, recherche/fusion,
                       saisie, page des suggestions de titre, page artiste)
    battle/            face-à-face de deux musiques ou deux artistes (scène, vue, vote,
                       battle_listen : écoute, avis, cadre du choix au clavier)
    ranking/           tier list (noms ou images : ranking_tiles.c), leaderboard (ex æquo
                       « - »), départage (ranking_ties, _drag, _order), reset (_reset)
    settings/          paramètres (aide au survol)
    playlist/          édition d'une playlist (nom, titres cochés, suppression)
    boards/            gestion des classements personnalisés (liste, règles, ajout)
    download/          écran de téléchargement (adresse, options : durée, playlists de
                       réception, lien ; progression, résumé) et liste par-dessus
                       (download_popup : hors durée / refusés / retirés)
    duplicates/        doublons : analyse du son une musique par tick, groupes
                       (_row, _block), fusions à la main et automatiques (_merge),
                       fichiers au son identique (_same), tri après un choix (_refresh)
    pages/             morceaux d'écran réutilisables (artist_page, category_bar,
                       board_bar : classement + musiques/artistes, image_box : import,
                       like_buttons : J'aime / Je n'aime pas, listen_bar : écoute hors
                       lecteur, search_field : champ de recherche)
src/ui/                tables d'éléments (atlas), collisions, formes, widgets,
                       widgets image / infobulle (widget_help : seulement si l'aide au
                       survol est activée), cache des textures d'images
src/elo/               catégories, calcul Elo, score (TOTAL), pool d'une arène, paires,
                       reset (ligne « reset » dans battles.log)
src/audio/             lecteur (un seul sfMusic ouvert), file de lecture, temps d'écoute,
                       écoute en battle (preview.c : second sfMusic, jamais compté)
src/download/          téléchargement par yt-dlp : lancement (liste ou vidéo), suivi à
                       chaque tick, lecture de la liste, tri des vidéos, ajout du mp3,
                       titres hors durée, titres retirés d'une playlist du site, bilan
                       (download_report.c)
src/library/           chargement / rafraîchissement / destruction de la bibliothèque
    file/              fichiers sur disque : validation mp3, empreinte audio, scan du
                       dossier, durées, fichiers au son identique à un autre
                       (same_files.c : même empreinte, gardés à part)
    id3/               lecture des tags ID3 et de la pochette (APIC)
    images/            images des musiques/artistes : miniatures, import, nettoyage du
                       chemin, réparation unique des images décalées (cover_check.c)
    playlists/         playlists, vue des musiques présentes, tri (critère, sens,
                       ordre personnalisé), filtre par texte (view_filter.c)
    boards/            classements, règles inclure/exclure, appartenance d'une musique,
                       classement auto par playlist, ordre de départage
    likes/             avis (+1/-1) et playlists auto « Titres likés / dislikés »
    suggest/           suggestions de titre (règles indépendantes, sûres seulement),
                       états cacher/supprimer, application (titre + artistes)
    songs/             chansons, empreintes connues, résolution des alias, édition, fusion,
                       suppression des fichiers (demandée seulement)
    videos/            vidéo d'origine d'un fichier (id), titres refusés, exclus, adresses
                       liées aux playlists
    duplicates/        doublons : titre nettoyé, artistes (uploader compris), paires
                       regroupées (dup_groups.c), enveloppe sonore et comparaison,
                       paires « pas un doublon »
    artists/           artistes, liens, noms affichés, stats et détail par artiste
src/data/              lecture/écriture TSV, migration, journaux
src/utils/             UTF-8, chaînes, découpage en mots, saisie de texte (avec Ctrl+V)
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
  sauf l'édition et la gestion des classements, qui remontent d'abord leurs sous-pages, et
  le téléchargement, qui ferme d'abord sa liste par-dessus ;
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
  chaque tick : ni thread ni variable globale. Annulé à la fermeture.
  Plusieurs liens séparés par `;` (découpés par `stwa_delimiter`, `src/utils/str_split.c`,
  repris de la wlib de l'auteur) forment une file : un seul yt-dlp à la fois
  (`src/download/download_queue.c`). Chaque lien est d'abord **listé** (`--flat-playlist
  -O`, une ligne par vidéo : id, durée, état live, adresse, titre ; rien n'est téléchargé),
  puis chaque vidéo est triée (`download_sort.c`) : déjà dans `music/` (retrouvée par son
  id, `song_by_video`) → seulement ajoutée aux playlists ; privée, supprimée ou live →
  ignorée ; hors de la durée choisie → mise de côté (`flagged`) ; sinon téléchargée **une
  par une**. Une vidéo finie est ajoutée seule à la bibliothèque (`download_add.c`, sans
  rescanner tout le dossier) puis aux **playlists de réception** (`dl_options_t.targets`,
  la playlist ouverte dans la bibliothèque est présélectionnée). Un échec est compté et la
  file continue. Options copiées au démarrage d'une file (`dl_options_t`), donc modifier
  l'écran ne change pas une file en cours.
  **Durée min / max** (vide = sans limite) : les titres hors plage attendent une décision
  dans la liste « Hors durée » (ouverte seule à la fin de la file, ou à la main) :
  télécharger (rejoint la file, ou en démarre une sans lien) ou refuser. Un refus est gardé
  dans `rejected.tsv` et écarte la vidéo **tant qu'elle reste hors de la plage**. Revenue
  dans la plage, elle n'est pas téléchargée seule : elle repasse dans « Hors durée »
  (« (refusé avant) ») ; « Télécharger » efface le refus, « Refuser » l'écarte pour de bon
  (`in_range`, annulable dans « Refusés »).
  **Lien adresse ↔ playlist** (un seul lien, une seule playlist de réception) : gardé dans
  `playlist_sources.tsv` avec la plage et l'option « Signaler les titres retirés » ;
  « Synchroniser » (bibliothèque, onglet de la playlist) rouvre le téléchargement rempli et
  le lance (`app->sync_playlist`). Les titres de la playlist locale dont la vidéo n'est plus
  listée sont proposés (garder / retirer de la playlist / supprimer le fichier, demandé
  deux fois) ; seulement si la liste a réussi et n'est pas vide, et jamais un titre sans
  vidéo connue. Progression « élément x / n » sur les vidéos retenues. Bascule « Cookies
  firefox » (`DL_BROWSER`, session seulement) pour les playlists privées
  (`--cookies-from-browser`). Taille et temps restant de toute la file estimés à partir de
  la moyenne des éléments finis, lus dans les lignes de progression de yt-dlp
  (`src/download/download_stats.c`, affichage `download_stats_view.c`).
  Fichiers nommés `titre [id].mp3` (deux vidéos au même titre ne s'écrasent plus,
  `--no-overwrites`) ; l'id YouTube est retiré du nom affiché (`scan_music.c`). Avant
  chaque file, `data/downloads.archive` est réécrit depuis les mp3 présents (tag `purl`
  écrit par yt-dlp, gardé dans `fprint_t.source`) : yt-dlp saute ces vidéos sans les
  télécharger (`--download-archive`), l'écran compte les vidéos ignorées.
- **Doublons** (scène `DUPLICATES`, depuis l'accueil, ou depuis le résumé d'un téléchargement
  avec `app->dup_since` = début de la file : seulement les nouveaux titres). Les paires
  trouvées sont regroupées (A ~ B et B ~ C = un groupe de trois). Deux étapes :
  1. métadonnées (`src/library/duplicates/`) : même cœur de titre (sans « Artiste - »,
     parenthèses/crochets, featurings, « official video »…), mêmes mots de version (remix,
     live, slowed, extended… : un remix n'est pas l'original), un artiste en commun (liens,
     tag = souvent la chaîne, nom avant « - » ; « - Topic », « VEVO », « Official » retirés),
     durées à 15 s près ;
  2. son, seulement pour ces paires : enveloppe = volume toutes les 100 ms (décodage
     `sfSoundBuffer`, un fichier par tick, pas au-delà de 20 min), comparée par corrélation
     des **variations** de volume avec un décalage jusqu'à 16 s. Mesuré sur la bibliothèque :
     musiques différentes < 30, mêmes enregistrements 74 à 99. ≥ 70 « même enregistrement »,
     40 à 69 « probable », < 40 écartée (deux homonymes). Cache `envelopes.tsv`.
  Chaque musique est comparée à la **référence** du groupe : la version auto-générée par
  YouTube (« Auto-generated by YouTube. » dans la description, `song_t.autogen`, relu à
  chaque scan), sinon la plus écoutée. Chaque ligne dit « Auto-généré » ou « Clip / autre
  upload » et peut être écoutée (`preview.c`, rien n'est compté). Un autre upload au son
  identique à ≥ 99 % (`DUP_AUTO`) à une version auto-générée y est **fusionné tout seul** à
  la fin de l'analyse.
  Choix : « Garder celui-ci » (les autres y sont fusionnés, `song_merge`), avec bascule
  « Supprimer le fichier en trop » (gardée dans `settings.tsv`, vaut aussi pour les fusions
  automatiques), ou « Pas un doublon » (la ligne sort du groupe, `not_duplicates.tsv`).
  Deux fichiers au son identique à l'octet (même empreinte, deux vidéos du même
  enregistrement) : le second n'est pas une musique, il est gardé dans `lib->same_files`
  (sa vidéo compte comme présente) et l'écran propose de l'effacer (« Fichiers
  identiques », demandé deux fois).
- **Échap** remonte aussi : classements (fenêtre de reset), bibliothèque (fenêtre des
  actions, puis mode sélection). Les écrans avec saisie gèrent Espace eux-mêmes.
- **Recherche** (`search_field.c`, `app->search`, vidée à chaque changement d'écran) :
  bibliothèque (la file de lecture suit le filtre, `player_t.filter`), doublons,
  classements (trois onglets, vrai rang gardé), liste du téléchargement. Clic dessus = elle
  reçoit les touches (Espace compris), Entrée ou un clic ailleurs les rend ; Échap la vide
  d'abord. Le glisser-déposer est coupé pendant une recherche.
- **Arène** (`app->arena`) : type (musiques/artistes) + classement + catégorie, partagée par
  les scènes battle et classements.

## Données (`data/`, format TSV v2)

Les fichiers mp3 ne sont **jamais modifiés** ; tout ce qui est édité vit dans `data/`.
Rien n'est jamais supprimé : une musique retirée garde ses stats (`present = 0`). Seule
exception, demandée explicitement : « Supprimer le fichier » (titre retiré d'une playlist du
site), « Supprimer le fichier en trop » (doublon fusionné, aussi automatiquement si la
bascule est sur oui), « Effacer ce fichier » (son identique), « Exclure » et « Supprimer du
lecteur » (mode sélection, édition) effacent le mp3 de `music/` (`song_delete_files`,
ligne dans `edits.log`) ; la musique garde ses stats et passe `present = 0`.

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
| `playlist_sorts.tsv` | tri choisi par playlist (0 = « Tous les titres ») : critère (par nom), asc/desc |
| `custom_orders.tsv` | ordre personnalisé de chaque playlist (ids bruts), à part de `playlist_songs.tsv` |
| `boards.tsv` / `board_rules.tsv` | classements personnalisés et leurs règles (include/exclude, all/playlist/artist/song) |
| `downloads.archive` | vidéos YouTube déjà dans `music/` pour yt-dlp (`youtube <id>`), réécrit avant chaque téléchargement (sécurité : les vidéos présentes sont déjà écartées après la liste) |
| `rejected.tsv` | vidéos refusées pour leur durée : id, durée (s), date, titre, refusée aussi dans la plage (colonne absente des anciens fichiers) |
| `playlist_sources.tsv` | adresse liée à une playlist : playlist, adresse, durée min/max (-1 = aucune), signaler les retirés |
| `envelopes.tsv` | enveloppe sonore d'un fichier (empreinte → octets en hexadécimal, vide si illisible), écrite en quittant l'écran des doublons |
| `not_duplicates.tsv` | paires marquées « Pas un doublon » (ids bruts) |
| `tie_orders.tsv` | ordre de départage des ex æquo par classement et type (ids bruts, position) |
| `likes.tsv` | avis : type, id brut, like/dislike (titres seulement pour l'instant) |
| `excluded.tsv` | vidéos exclues, jamais retéléchargées : id, date, titre |
| `downloads.log` | bilan de chaque file : date, statut (added/present/excluded/refused/flagged/unavailable/failed/interrupted), titre, détail |
| `suggestions.tsv` | suggestions de titre cachées ou supprimées : id brut, règle, texte retiré, état |
| `settings.tsv` | paramètres (clé, valeur) : `help` = aide au survol, `dup_remove` = bascule « Supprimer le fichier en trop » des doublons |
| `images/.covers_checked` | marque la réparation unique des images décalées (faite) |
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
  Le lecteur joue dans la file de la playlist choisie (`player_queue`), dans l'ordre affiché :
  liste et file passent toutes deux par `playlist_sorted_view` (tri fusion stable écrit à la
  main, valeurs inconnues toujours en fin). Critères : ajout à la playlist (= ordre de
  `playlist_songs.tsv`, les ajouts se font en fin), personnalisé (glisser-déposer), titre,
  artiste, durée, écoute, ajout au logiciel (`first_seen`), parution (tag année). Défaut :
  ordre de la playlist, titre pour « Tous les titres ». Durée mesurée une fois par CSFML
  pour chaque musique qui n'en a pas (`durations.c`, -1 si illisible), puis sauvegardée. Supprimer une
  playlist retire les règles de classement qui l'utilisent.
- **Images** : la pochette ID3 est extraite une fois au chargement si l'image n'existe pas ;
  remplaçable depuis l'édition (chemin collé) ou reprise du mp3. Image d'une fusion = celle
  de la musique résolue.
- Écriture dans `<fichier>.tmp` puis `rename()` (pas de corruption en cas de crash).
- Sauvegarde : au lancement, toutes les 30 s, à chaque changement de musique, à la fermeture
  (y compris Ctrl+C).
- L'ancien format v1 est converti automatiquement (originaux gardés en `*.v1.bak`).

## Plan de développement

### Phase 4.8 — Deuxième liste de l'auteur (faite, `data/mp3player_suggestions.txt`)
- **Doublons** : groupes de plus de deux (bug des trois « My Heart »), version
  auto-générée conseillée et fusion automatique à ≥ 99 %, écoute de chaque version,
  recherche, fichiers au son identique (bug « Same file twice », plus de message).
- **Recherche** : bibliothèque, doublons, classements, liste du téléchargement.
- **Battle** : clic droit sur une carte de musique = son édition ; l'édition gagne
  « Supprimer du lecteur » (demandé deux fois) à côté d'« Exclure ».
- **Téléchargement** : un titre refusé revenu dans la durée est redemandé, plus téléchargé
  tout seul.

### Phase 4.7 — Liste de suggestions de l'auteur (faite, `data/mp3player_suggestions.txt`)
- **Bugs** : Retour arrière maintenu (répétition activée, filtrée dans `keys.c`) ; images
  décalées d'une musique (35 images du 2026-11-20, réparées une fois par `covers_repair`,
  cause non retrouvée dans le code actuel).
- **Catégories** : + « Meme material » et « Agression auditive », hors TOTAL et hors aléatoire
  (`category_rated`).
- **Battle** : ←/→ choisit une carte, Entrée vote ; écoute du début avec barre cliquable
  (le lecteur est mis en pause puis reprend, rien n'est compté) ; avis sur les cartes.
- **Classements** : toutes les musiques du pool (1500 sans battle), tri Elo → % victoires →
  victoires → ordre de départage (un par classement et type, onglet « Départage » :
  glisser-déposer, par titre, par artiste, aléatoire). Rang partagé « - » si même Elo
  arrondi. Tiers S/A/B/C/D/F (10/20/25/20/15/10 %) parmi ceux qui ont une battle, plus
  « Non classé ». Un classement auto par playlist (id = -id de la playlist, jamais
  enregistré). Reset (ce classement ou tout, taper « Confirm »).
- **Avis** : J'aime / Je n'aime pas (bibliothèque, édition, page artiste, battle), deux
  playlists auto non modifiables à la main.
- **Exclusion** (édition, sélection, liste « Hors durée ») et **bilan** de chaque file
  (fin ou arrêt), onglets « Exclus » et « Bilan » de la liste du téléchargement.
- **Suggestions de titre** : préfixe/suffixe artiste connu ou uploader, crochets de bruit
  (official video, HD, lyrics…), licences (CC-BY, No Copyright…), guillemets, hashtags de
  fin, « Prod by », featuring/remix (l'artiste est ajouté, ou change de rôle s'il est déjà
  là ; un nom « A, B » est découpé). Pas d'album/série/BO (impossible d'être sûr). Une ligne
  sous le titre (« Tout appliquer ») et une page : Appliquer / Cacher / Supprimer.
- **Mode sélection** (bibliothèque) : cases, Maj+clic, tout l'onglet / aucun, fenêtre
  « Actions… » : avis, playlist (ajouter/retirer), suggestions, insérer un texte (début,
  fin, après n caractères, avec exemple), image pour tous, pochette du mp3, exclure,
  supprimer du lecteur (les deux demandés deux fois, un seul rescan).
- **Paramètres** : aide au survol activable (`widget_help`).

### Phase 4.6 — Téléchargement par playlist, synchronisation, doublons (faite)
Liste avant téléchargement, durée min/max et refus, playlists de réception, lien adresse ↔
playlist (« Synchroniser », titres retirés), écran des doublons (métadonnées + son).

### Phase 4.5 — Playlists, classements personnalisés, battles d'artistes, images (faite)
- Catégories : **Tout confondu** (jugement libre, indépendant, catégorie principale, jamais
  tirée au hasard, exclue de TOTAL), Mélodie, Production, Émotion, Rejouabilité,
  Nostalgie, puis (4.7) Meme material et Agression auditive (hors TOTAL et hors aléatoire).
  Mêmes catégories pour les artistes.

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
- **Tier list** et **leaderboard** (Elo, battles, % de victoires, défilement) : voir
  phase 4.7 pour les tiers actuels et le classement de toutes les musiques. Accès depuis
  l'accueil et la bibliothèque.
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
- Doublons : afficher leur nombre dans le résumé d'un téléchargement (il faudrait lire le
  son avant) ; aujourd'hui un bouton ouvre l'écran limité aux nouveaux titres.

## Références
- Boucle à tick : `/home/ceh0s/EPITECH/tek1/G-ING/wolf3d/src/update/update_events.c`
- Placement par tableau de floats + atlas : `/home/ceh0s/EPITECH/tek1/G-ING/myworld/src/interface.c`
