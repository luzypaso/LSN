#! /bin/sh

#Funzione per eseguire il ciclo di 16 simulazioni (da i=0 a i=15)
run_simulations() {
    H_VAL=$1
    PREFIX=$2
    TEMP=2.0

    #Imposta il valore di H nella prima riga
    #Usa awk per riscrivere la riga mantenendo la formattazione
    awk -v h="$H_VAL" 'NR==1 {$4=h} {print}' ../INPUT/input.dat > temp.dat && mv temp.dat ../INPUT/input.dat

    #Resetta il RESTART a 0 per la prima simulazione del blocco
    sed -i "2s/.*/RESTART \t\t\t   0/" ../INPUT/input.dat

    for i in $(seq 0 15); do

        #Gestione degli steps: per le prime 8 NSTEPS = 20000, e per le seconde 7 NSETPS = 40000
        if [ $i -lt 8 ]; then
            NSTEPS=20000
        else
            NSTEPS=40000
        fi

        #Aggiorna NSTEPS e TEMP nel file di input
        sed -i "9s/.*/NSTEPS \t\t\t   ${NSTEPS}/" ../INPUT/input.dat
        sed -i "3s/.*/TEMP \t\t\t   ${TEMP}/" ../INPUT/input.dat

        ./simulator.exe

        #Salvataggio dei file di output numerati
        if [ "$PREFIX" = "H0" ]; then
            mv ../OUTPUT/specific_heat.dat "../OUTPUT/GIBBS/specific_heat_${i}.dat"
            mv ../OUTPUT/susceptibility.dat "../OUTPUT/GIBBS/susceptibility_${i}.dat"
            mv ../OUTPUT/total_energy.dat "../OUTPUT/GIBBS/total_energy_${i}.dat"
        elif [ "$PREFIX" = "H002" ]; then
            mv ../OUTPUT/magnetization.dat "../OUTPUT/GIBBS/magnetization_${i}.dat"
        fi

        #Prepara la configurazione per le simulazioni successive
        cp ../OUTPUT/CONFIG/config.spin ../INPUT/CONFIG/config.spin
        sed -i "2s/.*/RESTART \t\t\t   1/" ../INPUT/input.dat

        #Diminuisce la temperatura di 0.1
        TEMP=$(awk "BEGIN {print $TEMP - 0.1}")
    done
}

#Blocco H = 0
run_simulations 0 "H0"

#Blocco H = 0.02
run_simulations 0.02 "H002"