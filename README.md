# Lintracker

Programmes C++ d'un robot suiveur de ligne piloté par une Raspberry Pi équipée d'une carte Sense HAT.

Projet réalisé en binôme en 1re année de BUT GEII (IUT de Toulouse, 2025-2026), dans le cadre d'une SAÉ. Le robot porte deux capteurs optiques qui distinguent la piste blanche de la ligne noire, et deux moteurs commandés en PWM.

## Les deux programmes

### `suivi-de-ligne` : conduite autonome

Le robot suit la ligne seul, à une vitesse maximale saisie au lancement.

- **Lecture des capteurs filtrée :** chaque capteur est lu six fois, à 10 ms d'intervalle, et l'état retenu est celui de la majorité. Un reflet ou un faux contact isolé ne fait donc pas dévier le robot.
- **Décision :**

| Capteur gauche | Capteur droit | Action |
|---|---|---|
| blanc | blanc | avance tout droit |
| noir | blanc | tourne d'un côté : une roue à pleine vitesse, l'autre à 20 % |
| blanc | noir | tourne de l'autre côté |
| noir | noir | s'arrête : fin du parcours |

### `pilotage-clavier` : course en conduite manuelle

Le robot est piloté au clavier, depuis la console de la Raspberry Pi.

- **Commandes :** la flèche haut accélère par paliers, la flèche bas ralentit, les flèches gauche et droite font tourner le robot en divisant par deux la vitesse d'une roue. L'état et la dernière touche s'affichent en temps réel dans la console.
- **Arrivée :** le programme surveille les deux capteurs pendant toute la course. Quand ils voient tous les deux la ligne noire d'arrivée, le robot s'arrête et la matrice de LED de la Sense HAT affiche un drapeau.

## Matériel

| Élément | Raccordement |
|---|---|
| Capteur de ligne gauche | GPIO 16 |
| Capteur de ligne droit | GPIO 17 |
| Moteur gauche | canal PWM 0 |
| Moteur droit | canal PWM 1 |
| Matrice de LED 8 × 8 | carte Sense HAT |

## Compiler et lancer

Sur la Raspberry Pi, avec la bibliothèque [libsensehat-cpp](https://github.com/platu/libsensehat-cpp) installée :

```bash
cd suivi-de-ligne      # ou pilotage-clavier
make
./suivi_de_ligne       # ou ./pilotage_clavier
```

## Crédits

La bibliothèque libsensehat-cpp et le modèle de départ des programmes ont été fournis par l'IUT.
