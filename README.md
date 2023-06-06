NAPOMENA: Sljedeći README.md opisuje zahtjeve i upute za pokretanje aplikacije napisane u C++ jeziku.

# Aplikacija za obradu datoteka

Ova aplikacija napisana je u C++ jeziku i služi za obradu CSV datoteka s informacijama o osobama. Aplikacija obavlja čitanje podataka iz ulazne datoteke te formatiranje i ispis podataka o osobama u tabličnom obliku te generira novu CSV datoteku s dodatnim informacijama u heksadekadskom obliku.

## Zahtjevi

Da biste uspješno pokrenuli aplikaciju, potrebno je zadovoljiti sljedeće zahtjeve:

- Kompajler za C++ jezik (npr. GCC, Clang, Visual C++ itd.)
- Operacijski sustav koji podržava C++ izvršavanje (npr. Windows, Linux, macOS)

## Pokretanje aplikacije

Slijedite ove korake kako biste pokrenuli aplikaciju:

1. Preuzmite izvorni kod aplikacije, uključujući obje datoteke: `generate-data.cpp` i `file-processing.cpp`. `generate-data.cpp` je opcionalni dio aplikacije - koristi se za generiranje CSV datoteke s podacima o osobama, a putanja te datoteke može se koristiti u glavnom dijelu aplikacije `file-processing.cpp`.

2. Kompajlirajte izvorni kod pomoću odgovarajućeg C++ kompajlera. Na primjer, za GCC možete koristiti sljedeću naredbu u terminalu:

   ```
   g++ generate-data.cpp -o generate-data
   g++ file-processing.cpp -o file-processing
   ```

   Ova naredba će stvoriti izvršne datoteke `generate-data` i `file-processing`.

3. Pokretanje aplikacije ovisi o korištenom terminalu:

   - **Bash (Linux ili macOS)**: U bash terminalu, pokrenite izvršne datoteke koristeći `./` ispred naziva. Na primjer:

     ```
     ./generate-data
     ./file-processing
     ```

   - **Command Prompt (Windows)**: U Command Promptu (cmd), možete pokrenuti izvršne datoteke bez `./`. Na primjer:

     ```
     generate-data
     file-processing
     ```

   - **Drugi terminali**: Ovisno o korištenom terminalu, možda će biti potrebno koristiti odgovarajuću sintaksu za pokretanje izvršnih datoteka. Obično se koristi `./` ispred naziva izvršne datoteke.

4. Opcionalno: Pokrenite `generate-data` izvršnu datoteku kako biste generirali početnu CSV datoteku. Ova datoteka sadrži generirane podatke o osobama.

   ```
   ./generate-data
   ```

5. Pokrenite `file-processing` izvršnu datoteku kako biste obradili generiranu CSV datoteku.

   ```
   ./file-processing
   ```

   Aplikacija će zatražiti unos putanje do ulazne CSV datoteke koju želite obraditi. Unesite putanju i pritisnite Enter. Možete unesti putanju `input/data.csv` ako ste pokrenuli program `generate-data`.

6. Aplikacija će prikazati formatirane podatke o osobama u tabličnom obliku i generirati novu CSV datoteku s dodatnim informacijama u heksadekadskom obliku. Nova datoteka bit će spremljena u direktorij "output" pod nazivom "data_TIMESTAMP.csv", gdje će "TIMESTAMP" predstavljati trenutno vrijeme generiranja datoteke.

7. Rezultati obrade bit će prikazani na zaslonu, a nova CSV datoteka bit će stvorena u direktoriju "output".

## Napomene

- Generirana ulazna CSV datoteka sadrži 10 zapisa s nasumično generiranim imenima, prezimenima i datumima rođenja.
- Podaci o osobama bit će formatirani i prikazani u tabličnom obliku. Također će biti generirani heksadekadski oblici imena, prezimena i datuma rođenja.
- Izvorna CSV datoteka koju ste odabrali bit će validirana prije obrade. Samo datoteke s ekstenzijom ".csv" bit će prihvaćene.
- Ako se direktorij "output" ne nalazi u istom direktoriju kao izvršna datoteka `file-processing`, aplikacija će automatski stvoriti taj direktorij prije spremanja rezultata (kao i za direktorij "input" ako se pokrene izvršna datoteka `generate-data`).