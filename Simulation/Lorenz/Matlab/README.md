# Simulazione MATLAB della traiettoria

Il file `inpudataforsimulation.m` contiene gli input dei parametri del sistema, la posizione iniziale ed i parametri di intergazione da passare alla funzione lorenz_attractor per la simulazione della traiettoria.
Modificare i vari parametri per ottenere traiettorie diverse.

## Range degli input

Dalla teoria, l'attrattore di Lorenz assume comportamento caotico se i parametri $\rho$, $\sigma$ e $\beta$ assumo i valori di default di $28$, $10$, $8/3$, oppure valori molto vicini.
dato che l'attrattore è globale, non ci sono limitazioni sulla posizione iniziale, in un tempo finito la traiettoria converge sempre all'attrattore.
