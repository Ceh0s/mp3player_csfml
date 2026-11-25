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

### Norme Epitech (plus obligatoire depuis le 2026-11-24)
Décision de l'auteur : la norme (`/home/ceh0s/Documents/epitech_c_coding_style.pdf`,
vérifiée par epiclang via `make debug`) n'est **plus imposée**. La respecter coûtait du
temps et des ressources mieux employés ailleurs. Les écarts n'ont plus à être justifiés ni
signalés.
Restent **appréciés** (sans obligation) : un code lisible et facile à comprendre, des
fichiers qui font chacun leur tâche avec un nom qui la dit, des sous-dossiers par thème,
l'en-tête Epitech avec une description utile, pas de variable globale non `const`.

**Dépendance externe acceptée** : le téléchargement lance le programme `yt-dlp` (avec
`ffmpeg`), installé à part (`sudo pacman -S yt-dlp`). Ce n'est pas une bibliothèque liée :
sans lui, l'appli fonctionne et l'écran de téléchargement l'indique. De même, `curl`
(présent sur Arch, pacman en dépend) lit les pages d'une playlist sur YouTube Music ;
sans lui, aucune vidéo n'est remplacée (voir Téléchargement).

### Avant de rendre la main
1. `make re` sans warning (`-Wall -Wextra`).
2. `make debug` (epiclang) : facultatif, la norme n'est plus imposée.
3. `valgrind --leak-check=full ./MP3player` : 0 erreur, 0 fuite.
4. Ne jamais commiter sans accord. `music/` et `data/` sont ignorés par git.

## Architecture

```
main.c
include/   app.h (app_t)  scene.h (scènes, home_t, libscene_t, edit_t, battle_t, ranking_t,
           pledit_t, boards_t, dlscene_t, dupscene_t, setscene_t, selection_t)  song.h
           (song_t, fprint_t, artist_t, link_t, playlist_t, board_t, rule_t, rejected_t,
           source_t, note_t, period_t, video_alias_t, envelope_t, not_dup_t, dup_info_t, dup_pair_t, like_t, tie_order_t,
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
    library/           liste des musiques + barre de lecture (pochette du titre en
                       cours, clic = sa page) + onglets de playlists
                       + barre de tri et glisser-déposer (library_scene.c, _list,
                       _view, _tabs, _bar, _layout, _sort, _drag), avis (_likes),
                       mode sélection (_select, _select_view) et ses actions en lot
                       (_actions, _action_rows, _bulk, _insert)
    edit/              écran d'édition (page principale, lignes d'artistes, recherche/fusion,
                       saisie, page des suggestions de titre, page artiste, page « Découverte » :
                         edit_notes, edit_periods, edit_periods_save)
    battle/            face-à-face de deux musiques ou deux artistes (scène, vue, vote,
                       battle_listen : écoute, avis, cadre du choix au clavier)
    ranking/           tier list (noms ou images : ranking_tiles.c ; clic sur un tier =
                       lui seul en grand), leaderboard (ex æquo « - », vue par défaut),
                       départage (ranking_ties, _drag, _order), reset (_reset),
                       historique des battles (ranking_history.c : dernière en
                       haut, Elo et places avant / après en vert / rouge / gris,
                       face à face ; aussi ouvert par le bouton « Historique » de
                       la battle), clic droit = page de la musique ou de
                       l'artiste, état gardé au retour (_open, app->ranking_keep)
    settings/          paramètres (aide au survol)
    stats/             statistiques d'écoute : leaderboard des titres ou artistes, par
                       playlist, entre deux dates, Elo à côté si voulu (_bar, _dates,
                       _build, _view)
                       ; clic droit sur une ligne = page de la musique ou de
                       l'artiste (pages/page_open.c)
    playlist/          édition d'une playlist (nom, titres cochés, suppression, import des dates :
                         pledit_takeout.c)
    boards/            gestion des classements personnalisés (liste, règles, ajout)
    download/          écran de téléchargement (adresse, options : durée, playlists de
                       réception, lien ; progression, résumé) et liste par-dessus
                       (download_popup : hors durée / refusés / retirés / exclus /
                       indisponibles / bilan ; refusés et exclus : _refused_view,
                       _excluded_view, archivage commun : _archive_view ; noms
                       d'artistes à vérifier : _split_view)
    duplicates/        doublons : analyse du son une musique par tick, groupes
                       (_row, _block), fusions à la main et automatiques (_merge),
                       fichiers au son identique (_same), tri après un choix (_refresh)
    pages/             morceaux d'écran réutilisables (artist_page, category_bar,
                       board_bar : classement + musiques/artistes, image_box : import,
                       like_buttons : J'aime / Je n'aime pas, listen_bar : écoute hors
                       lecteur, search_field : champ de recherche)
src/ui/                tables d'éléments (atlas), collisions, formes, widgets, icônes de
                       l'atlas (widget_icon.c : contour, disque plein = état actif,
                       estompé = inactif), ligne de titre (widget_song.c : titre en
                       gros, artistes dessous), « … » (text_fit.c), champ de
                       saisie (widget_field.c : curseur cliquable, texte qui défile),
                       polices de secours (text_fallback.c : un caractère absent de
                       font.ttf prend Noto Sans JP puis FreeSerif, caractère par
                       caractère ; tout texte passe par text_draw / text_width /
                       text_char_x),
                       widgets image / infobulle (widget_help : seulement si l'aide au
                       survol est activée), cache des textures d'images
src/elo/               catégories, calcul Elo, score (TOTAL), pool d'une arène, paires,
                       reset (ligne « reset » dans battles.log), historique d'une
                       arène (elo_history.c : battles.log rejoué pour ce type et ce
                       classement, places comptées dans la poule actuelle)
src/audio/             lecteur (un seul sfMusic ouvert), file de lecture (aléatoire :
                       player_shuffle.c, boucle), temps d'écoute,
                       écoute en battle (preview.c : second sfMusic, jamais compté),
                       stop, ±10 s, volume et muet (player_controls.c)
src/download/          téléchargement par yt-dlp : lancement (liste ou vidéo), suivi à
                       chaque tick, lecture de la liste, tri des vidéos, ajout du mp3,
                       titres hors durée, titres retirés d'une playlist du site, bilan
                       (download_report.c), raisons des échecs en français (dl_reason.c),
                       ordre de la playlist du site (download_order.c), nouvelle
                       version d'un réupload retrouvée (download_reupload.c),
                       vidéo jouée à la place par YouTube Music (download_swap.c,
                       lecture des pages : swap_parse.c)
src/library/           chargement / rafraîchissement / destruction de la bibliothèque
    file/              fichiers sur disque : validation mp3, empreinte audio, scan du
                       dossier, durées, fichiers au son identique à un autre
                       (same_files.c : même empreinte, gardés à part), oubli des
                       fichiers effacés sans rescan (missing_files.c)
    id3/               lecture des tags ID3 et de la pochette (APIC)
    images/            images des musiques/artistes : miniatures, import, nettoyage du
                       chemin, réparation unique des images décalées (cover_check.c)
    playlists/         playlists, vue des musiques présentes, tri (critère, sens,
                       ordre personnalisé), filtre par texte (view_filter.c)
    boards/            classements, règles inclure/exclure, appartenance d'une musique,
                       classement auto par playlist, ordre de départage
    notes/             où et quand une musique a été trouvée (textes de l'auteur)
    periods/           périodes des musiques dans les playlists : suivi (periods_sync.c),
                       résumé (periods_sum.c), import Takeout (takeout.c, _songs)
    likes/             avis (+1/-1) et playlists auto « Titres likés / dislikés »
    suggest/           suggestions de titre (règles indépendantes, sûres seulement),
                       états cacher/supprimer, application (titre + artistes)
    songs/             chansons, empreintes connues, résolution des alias, édition, fusion,
                       suppression des fichiers (demandée seulement), oubli d'un
                       exclu (song_forget.c, _refs : références, _logs : journaux)
    videos/            vidéo d'origine d'un fichier (id), vidéos jouées sous un autre id
                       (video_alias.c), titres refusés, exclus
                       (musique d'un exclu retrouvée dans edits.log : excluded_song.c),
                       indisponibles, adresses liées aux playlists
    duplicates/        doublons : titre nettoyé, artistes (uploader compris), paires
                       regroupées (dup_groups.c), enveloppe sonore et comparaison,
                       paires « pas un doublon »
    artists/           artistes, liens, noms affichés, stats et détail par artiste,
                       renommage (artist_rename.c : un nom déjà pris = fusion), noms
                       en liste séparés (artist_split.c, choix gardés :
                       split_choices.c), ordre des artistes d'une musique
                       (link_order.c)
    stats/             statistiques : lecture de listens.log, sommes (compteurs ou
                       période), tri
src/data/              lecture/écriture TSV, migration, journaux
src/utils/             UTF-8 (pas à pas : utf8_step.c), chaînes, découpage en mots,
                       saisie de texte au curseur (text_caret.c : flèches, Début, Fin,
                       Suppr ; avec Ctrl+V)
assets/       atlas.png (25 icônes 256 px sur 10 colonnes : contours, puis les mêmes
              en disque plein à l'indice + NB_ICONS, enum icon_e de ui.h), fonds JPEG, font.ttf,
              font_jp.otf (Noto Sans JP Medium, licence OFL : japonais, chinois,
              cyrillique, grec), font_serif.otf (FreeSerif, GPL avec exception
              pour les polices : thaï, « ⧸ », maths), no_cover.png (image par
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
  **Durée min / max** (vide = sans limite, la dernière tapée est gardée jusqu'à la
  fermeture, `app->dl_range`) : les titres hors plage attendent une décision
  dans la liste « Hors durée » (ouverte seule à la fin de la file, ou à la main) :
  télécharger (rejoint la file, ou en démarre une sans lien) ou refuser. Un refus est gardé
  dans `rejected.tsv` et écarte la vidéo **tant qu'elle reste hors de la plage**. Revenue
  dans la plage, elle n'est pas téléchargée seule : elle repasse dans « Hors durée »
  (« (refusé avant) ») ; « Télécharger » efface le refus, « Refuser » l'écarte pour de bon
  (`in_range`, annulable dans « Refusés »).
  **Lien adresse ↔ playlist** (un seul lien, une seule playlist de réception) : gardé dans
  `playlist_sources.tsv` avec la plage et l'option « Signaler les titres retirés » ;
  « Synchroniser » (bibliothèque, onglet de la playlist) rouvre le téléchargement rempli et
  le lance (`app->sync_playlist`). **Ordre du site** : pour un seul lien vers une seule
  playlist, liste réussie de plus d'une vidéo, la playlist locale prend l'ordre de la liste
  de yt-dlp à la fin de la file (pas si elle est annulée) ; les titres sans vidéo listée
  restent après, dans leur ordre. L'ordre personnalisé n'est pas touché.
  **Réuploads** : une vidéo « not available » (déjà connue ou qui vient d'échouer) est
  cherchée parmi les musiques présentes avec les règles des doublons (même cœur de titre
  et version, chaîne de la vidéo parmi les artistes, durée à 15 s près ; la liste de
  yt-dlp donne la chaîne même pour une vidéo indisponible). Une seule correspondance, ou
  une seule auto-générée : elle est ajoutée aux playlists à la place, à sa place dans
  l'ordre du site (« réupload : remplacé par … » dans le bilan).
  **Versions jouées par YouTube Music** : une playlist garde l'id d'une vidéo qui n'est
  plus servie, et le site joue une autre version à sa place ; yt-dlp ne liste que
  l'ancien id. Après la liste d'une adresse `list=`, l'étape `DL_STEP_SWAP` lance `curl`
  sur l'API web de YouTube Music (`youtubei/v1/browse`, client `WEB_REMIX`, pages de
  100, jeton `continuationCommand`, au plus 20 pages), page écrite dans
  `data/ytm_page.json` puis effacée. Chaque titre y donne l'id gardé (cible du bouton
  J'aime) puis l'id joué (`playlistItemData`) : l'entrée listée prend l'id joué
  (`dl_entry_t.swapped`, « version jouée par YouTube Music » dans le bilan), avant le
  repérage des titres retirés et le tri. L'ancien id est gardé s'il est déjà dans
  `music/`, ou si une autre version y est retrouvée par les règles des réuploads (sauf
  si c'est la version jouée) : pas de doublon téléchargé. API interne : si elle change
  ou échoue (réseau, playlist privée), rien n'est remplacé.
  Les titres de la playlist locale dont la vidéo n'est plus listée sont proposés (garder / retirer de la playlist / supprimer le fichier, demandé
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
- **Lignes de titre** (bibliothèque, classements, statistiques, doublons, playlists,
  titres retirés, recherche de l'édition) : titre en gros, artistes en gris dessous
  (`widget_song`), coupés par « … » si le paramètre `ellipsis` est actif. Les lignes de
  la bibliothèque font 64 px ; un clic est cherché par la position de la souris. Avis
  à droite de chaque ligne (les deux icônes au survol, l'avis donné toujours visible).
  Tri : bouton « Tri : … » qui ouvre la liste des critères, sens par une icône.
- **Échap** remonte aussi : classements (fenêtre de reset), bibliothèque (liste des
  tris, fenêtre des actions, puis mode sélection), statistiques (champ de date). Les écrans avec saisie gèrent Espace eux-mêmes.
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
Seule exception à la conservation des stats : **« Oublier »** (liste « Exclus » du
téléchargement, demandé deux fois, `song_forget`) efface tout d'une musique exclue dont
le fichier n'est plus là : musique, empreintes, enveloppes, liens, playlists, ordres,
avis, départage, paires, suggestions, règles, images, ses lignes de `listens.log` et
`battles.log` (Elo rejoués). Seule la vidéo reste dans `excluded.tsv`. Impossible si
d'autres musiques y sont fusionnées. Son id n'est jamais redonné (`next_song` dans
`settings.tsv`).

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
| `rejected.tsv` | vidéos refusées pour leur durée : id, durée (s), date, titre, refusée aussi dans la plage, archivée (« Vider la liste » ; colonnes absentes des anciens fichiers) |
| `playlist_sources.tsv` | adresse liée à une playlist : playlist, adresse, durée min/max (-1 = aucune), signaler les retirés |
| `envelopes.tsv` | enveloppe sonore d'un fichier (empreinte → octets en hexadécimal, vide si illisible), écrite en quittant l'écran des doublons |
| `not_duplicates.tsv` | paires marquées « Pas un doublon » (ids bruts) |
| `tie_orders.tsv` | ordre de départage des ex æquo par classement et type (ids bruts, position) |
| `likes.tsv` | avis : type, id brut, like/dislike (titres seulement pour l'instant) |
| `song_notes.tsv` | où et quand une musique a été trouvée (id brut, deux textes libres) |
| `playlist_history.tsv` | périodes d'une musique dans une playlist : playlist, id brut, début (0 = inconnu), fin (0 = en cours, -1 = inconnue), origine du début (unknown/auto/import/hand), « depuis le début » (auto/yes/no) |
| `video_aliases.tsv` | vidéo gardée par une playlist du site → vidéo jouée à sa place (lue à la synchronisation, sert à l'import Takeout) |
| `excluded.tsv` | vidéos exclues, jamais retéléchargées : id, date, titre, archivée (absente des anciens fichiers) |
| `unavailable.tsv` | anciennes versions de titres republiés (seul « This video is not available », souvent Home, NCS…) : id, date, titre, ligne d'erreur de yt-dlp, archivée (« Vider la liste », absente des anciens fichiers) ; sautées aux téléchargements suivants, « Retenter » dans l'onglet « Indisponibles » ; les autres échecs sont retentés |
| `downloads.log` | bilan de chaque file : date, statut (added/present/excluded/refused/flagged/unavailable/failed/interrupted), titre, détail |
| `suggestions.tsv` | suggestions de titre cachées ou supprimées : id brut, règle (par nom, dont `artist_first` / `title_first` = suppositions « X - Y »), texte retiré, état |
| `artist_splits.tsv` | noms d'artistes écrits en liste (« A, B ») : nom, `split` ou `keep` (garder entier ; aussi quand un nom à virgule est enregistré dans l'édition) |
| `settings.tsv` | paramètres (clé, valeur) : `help` = aide au survol, `dup_remove` = bascule « Supprimer le fichier en trop » des doublons, `shuffle` = lecture aléatoire, `repeat` = boucle (0 non, 1 file, 2 titre), `volume` (0 à 100), `mute`, `ellipsis` = « … » sur les textes trop longs, `next_song` / `next_artist` = premier id de musique / d'artiste jamais donné, `battle_match` = rencontres équilibrées en battle, `links_ordered` = réparation unique de l'ordre des artistes faite, `periods_started` = suivi des playlists commencé |
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
  Le lecteur joue dans la file de la playlist choisie (`player_queue`), dans l'ordre affiché
  (ou au hasard, le titre en cours en tête, ordre gardé quand la file est reconstruite ;
  boucle de la file ou du titre, boutons de la barre de lecture) :
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

### Phase 4.15 — Défilement fluide, tri par date d'ajout
- **Bug** : après une synchronisation, les titres ayant deux fichiers (doublons fusionnés sans
  supprimer le mp3) allaient après le dernier : l'ordre du site cherche maintenant la musique
  de chaque vidéo (`order_songs`), pas la vidéo de chaque musique.

### Phase 4.14 — Huitième liste de l'auteur (faite, `data/mp3player_suggestions.txt`)
- **Découverte** (bouton de la page d'une musique) : « Trouvée où » et « Date de découverte »
  (textes libres), et les passages de la musique dans chaque playlist (jamais, n passages,
  depuis quand, « depuis le début »), avec leurs dates modifiables. Copie de travail,
  écrite par « Enregistrer ».
- **Suivi des playlists** : `periods_sync`, appelé par `db_save_playlists`, compare le contenu
  des playlists aux périodes ouvertes ; aucun autre code n'a à le tenir à jour. Une période
  ne finit que sur un retrait de la playlist (pas quand le fichier disparaît). Moins de
  10 min entre ajout et retrait (ou retrait et remise) = erreur de clic, sans trace. Les
  titres déjà là au début du suivi ont une date inconnue. « Depuis le début » : ajout dans
  les 7 jours après la plus ancienne date de la playlist et jamais retiré, ou forcé.
- **Import Takeout** (écran d'une playlist) : YouTube garde la date d'ajout actuelle de chaque
  vidéo (pas les anciens passages), donnée par le .csv de la playlist exporté par Google
  Takeout. En-tête ignoré (il dépend de la langue). Une date tapée à la main n'est jamais
  remplacée ; une date déjà importée devenue plus récente = retrait puis réajout sur le site.
  Deux clics si moins de la moitié des vidéos sont dans la playlist. Synchroniser avant aide
  à retrouver les versions remplacées.
  Une date qui tombe dans un passage déjà terminé est ignorée (export fait avant un réajout).
  Testé sur un vrai export (Playlist du Gueux, 2026-11-25) : 309 vidéos lues, 308 datées,
  dont 10 par `video_aliases.tsv` ; Google met une espace devant les ids qui commencent
  par « - ».
- **Bibliothèque** : le tri « Ajout playlist » écrit la date d'ajout à droite.
- **Bug** : les anciens noms en liste (« John Summit, HAYLA ») et les artistes sans musique ne
  sont plus proposés (`artist_linked` : ajout d'artiste, règles de classement).

### Phase 4.13 — Septième liste de l'auteur (faite, `data/mp3player_suggestions.txt`)
- **Statistiques** : clic droit sur une ligne = page de la musique ou de l'artiste
  (`page_open`, aussi utilisé par les classements), défilement gardé au retour.
- **Bibliothèque** : « Lire » (onglet affiché depuis son premier titre, aléatoire coupé)
  et « Aléatoire » (ordre tiré à nouveau, premier titre compris, aléatoire activé),
  `player_start`.
- **Barre de lecture** : pochette du titre en cours avant son titre ; un clic sur la
  pochette, le titre ou les artistes ouvre sa page (`bar_open_song`).
- **Polices de secours** : japonais, thaï, cyrillique, « ⧸ », « – », « ’ »… s'affichaient
  en points (font.ttf n'a que 220 caractères) ; chaque caractère absent prend la première
  police de secours qui l'a (`text_fallback.c`). Les emojis restent sans rendu (CSFML
  ne dessine pas les emojis en couleur).
- **Historique des battles** (onglet « Historique » des Classements, bouton sur l'écran
  Battle) : heure, Elo et place de chaque côté avant / après (vert = gain, rouge =
  perte, gris = rien), face à face dans la catégorie depuis le dernier reset.
  `playlist_has_song` ne cherche plus chaque id de la playlist dans la liste des
  musiques (poule d'un classement de playlist : 500 → 18 ms).
- **Ligne d'artistes** (sous chaque titre) : « A, B, C » dans l'ordre des liens, sans
  « feat. » ni « (remix …) », comme YouTube Music ; les rôles restent sur la page de
  la musique et la page artiste.
- **Indisponibles** : « Vider la liste » et « Voir les archivés » comme Refusés / Exclus
  (les archivées restent sautées) ; « Tout retenter » ne vise que la liste affichée.
- **Bug** : l'ordre des artistes d'une musique s'inversait à chaque lancement
  (`db_parse_link` ajoutait chaque lien en tête) ; la liste est remise dans l'ordre du
  fichier (`links_reverse`), et l'ordre du tag est rétabli une fois (`links_order_repair`).

### Phase 4.12 — Sixième liste de l'auteur (faite, `data/mp3player_suggestions.txt`)
- **Battle** : paires au hasard par défaut, « Rencontres : équilibrées » en option.
- **Classements** : leaderboard par défaut ; clic sur un tier = lui seul en grand (molette,
  Échap ou nouveau clic pour revenir) ; clic droit = page de la musique / de l'artiste.
- **Page d'une musique** : « Écouter » joue dans le vrai lecteur (file « Tous les titres »,
  écoute comptée). Page artiste : « Renommer » (un nom pris réunit les deux artistes).
- **Suggestions** : featuring / remix crédités sans toucher au titre ; « X - Y » sans nom
  connu (republieur) : deux suppositions (X artiste ou Y artiste), jamais dans « Tout
  appliquer ».
- **Artistes en liste** (« C418, Baiyon », écrits ainsi par YouTube Music) : séparés tout
  seuls à chaque `library_refresh`, rôle gardé, « & » jamais coupé. Douteux (« The X »
  d'un seul mot, « & » dans la dernière partie, sauf si toutes les parties sont déjà des
  artistes) : onglet « À vérifier » du téléchargement, et « Séparer » dans l'édition.
- **Import** : un mp3 sans tag posé dans `music/` marche déjà (titre = nom du fichier,
  suppositions artiste / titre), vu au lancement suivant ; seuls les `.mp3` sont lus.

### Phase 4.11 — Cinquième liste de l'auteur (faite, `data/mp3player_suggestions.txt`)
- **Battle** : l'adversaire était tiré parmi les 3 premiers à Elo égal, donc toujours
  les premiers titres par ordre alphabétique (tous à 1500) ; tiré maintenant parmi
  tous les Elo aussi proches. Les cartes nomment la catégorie de leurs stats.
- **Icônes** (atlas refait, nouveau fond) : barre de lecture (stop, ±10 s, aléatoire,
  boucle, volume et muet), avis, tri, menu d'accueil, fermer / retour.
- **Lignes de titre** sur deux niveaux et « … » (bascule dans les Paramètres).
- **Exclus / Refusés** : « Vider la liste » archive, « Voir les archivés », « Oublier ».

### Phase 4.10 — Quatrième liste de l'auteur (faite, `data/mp3player_suggestions.txt`)
- **Ordre de la playlist importée** : celui du site (voir Téléchargement).
- **Réuploads** d'une playlist importée retrouvés dans la bibliothèque (Playlist du
  Gueux : 295 → 303 titres sur 308 ; 3 nouvelles versions absentes, 2 erreurs 403).
  « reversed » ajouté aux mots de version des doublons.
- **Nombre de titres** présents dans l'onglet affiché de la bibliothèque (« trouvés /
  total » pendant une recherche).
- Rangée du haut de la bibliothèque resserrée (« Statistiques » passait sous
  « Sélection »).
- **Versions jouées par YouTube Music** : « Dark World » et « Nights » paraissaient
  disponibles sur le site mais pas pour yt-dlp (anciens ids gardés par la playlist).
  Lus dans l'API de YouTube Music : 307 titres sur 308 (seul « Leaked flavor » est
  vraiment supprimé).

### Phase 4.9 — Troisième liste de l'auteur (faite, `data/mp3player_suggestions.txt`)
- **Gel** de plusieurs secondes après chaque choix : `song_build_artists` cherchait
  l'artiste de chaque lien avant de tester la musique (1,5 s → 10 ms) ; une suppression
  oublie seulement les fichiers effacés au lieu de relire tout `music/` (0,6 s).
- **Saisie** : curseur (flèches, Début, Fin, Suppr, clic), texte long qui défile.
- **Lecture** : aléatoire et boucle (non / file / titre), gardés dans `settings.tsv`.
- **Téléchargement** : plage de durée gardée pendant la session ; raisons des échecs
  traduites (Bilan, dernier échec, texte complet au survol) ; 473 des 489 échecs
  étaient « This video is not available » : d'anciennes versions de titres republiés
  (constaté par l'auteur), retenues dans `unavailable.tsv` et sautées (ce message
  seulement).
- **Statistiques** (accueil, bibliothèque) : voir la phase 5.

### Phase 4.8 — Deuxième liste de l'auteur (faite, `data/mp3player_suggestions.txt`)
- **Doublons** : groupes de plus de deux (bug des trois « My Heart »), version
  auto-générée conseillée et fusion automatique à ≥ 99 %, écoute de chaque version,
  recherche, fichiers au son identique (bug « Same file twice », plus de message).
- **Recherche** : bibliothèque, doublons, classements, liste du téléchargement.
- **Battle** : clic droit sur une carte de musique = son édition (même paire au retour,
  `battle_restore`) ; l'édition gagne
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
  flèches gauche/droite pour voter. K compté par musique et par catégorie. Paires tirées
  **au hasard** parmi les musiques présentes (depuis 4.12) ; bascule « Rencontres :
  équilibrées » (`battle_match`) : titres avec peu de battles d'abord, puis un adversaire
  parmi les Elo les plus proches (tous ceux à égalité avec le 3ᵉ).
- **Tier list** et **leaderboard** (Elo, battles, % de victoires, défilement) : voir
  phase 4.7 pour les tiers actuels et le classement de toutes les musiques. Accès depuis
  l'accueil et la bibliothèque.
- Toujours utiliser l'id **résolu** (`song_resolve`) : une fusion doit regrouper les Elo.
- Écrire `elo.tsv` (`song_id category rating battles wins`) et `battles.log` (rejouable).

### Phase 5 — Écran de statistiques (commencée en 4.9)
Fait : leaderboard par temps d'écoute (titres ou artistes = somme de leurs titres, tous
rôles), Général ou une playlist, écoutes et skips, Elo d'une catégorie à côté (classement
Général), tri au choix, recherche. Période « Du / au » : avec une date, seules les
écoutes de `listens.log` comptent (les seules datées) ; sans date, les compteurs (tout
l'historique, alias compris). Seules les musiques présentes sont listées.
À faire : albums, genres, historique, autres idées de l'auteur.

### Idées notées pour plus tard
- Reconnaissance audio (AcoustID / MusicBrainz) pour identifier automatiquement artiste et
  titre : demande Internet et des bibliothèques en plus.
- Onglets de playlists : défilement quand il y en a trop (ceux qui dépassent sont cachés).
- Page artiste accessible ailleurs que depuis l'édition d'une musique (pour l'image).
- Fusion d'artistes (les Elo d'artistes ne sont pas encore résolus).
- Retour éventuel sur Raspberry Pi 4.
- Doublons : afficher leur nombre dans le résumé d'un téléchargement (il faudrait lire le
  son avant) ; aujourd'hui un bouton ouvre l'écran limité aux nouveaux titres.

## Références
- Boucle à tick : `/home/ceh0s/EPITECH/tek1/G-ING/wolf3d/src/update/update_events.c`
- Placement par tableau de floats + atlas : `/home/ceh0s/EPITECH/tek1/G-ING/myworld/src/interface.c`
