# Synth-Project
L'objectif est de concevoir un synthétiseur avec de nombreux paramètres pour modifier le son. Le synthé sera contrôlable via un signal MIDI envoyé par un clavier ou un ordinateur. 

# Cahier des Charges
- Synthétiseur monophonique
- 4 formes d'onde de bases pouvant être mixés
- 1 entrée MIDI
- 1 sortie jack en mono conforme aux normes audio
- Tuning (1 potards)
- 2 boutons pour octavier (+ LED)
- Bouton on/off (+ LED)
- Volume (1 potard)
- Enveloppe ADSR (4 potards)
- Vibrato (2 potards + LED)
- Tremolo (2 potards + LED)
- Pitch bend + amplitude (2 potard + LED)
- Filtre universel
- Filtre low pass + résonance (2 potards)
- Filtre avance de phase (2 potards + éventuelle résonance)
- Distortion/Saturator (1 potard + LED)
- Wavefolder (1 potard + LED)
- Unisson pour signal triangle et carré (avec LFO)
- Plusieurs LFO pour volume (tremolo), unisson, filtre, wavefolder, type de son? (2 potards + LED chacun)

# TODO
- [ ]  Recopier schémas sur KiCad
- [ ]  Montage octocoupleur + condensateur (vérifier pas de clic)
- [ ]  Test remplacer VCR par MOSFET (saw generator)… plus stable ?
- [ ]  Trouver générateur plus stable (sinus si possible)
- [ ]  Commander multiplexeur
