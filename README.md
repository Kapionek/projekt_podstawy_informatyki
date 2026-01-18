Zbijak 2D - Physics-based Game
Dwuwymiarowa gra zręcznościowa dla dwóch graczy oparta na fizyce (physics-based) zrealizowana w języku C++ przy wykorzystaniu biblioteki SFML.

1. Wymagania systemowe
Do poprawnego skompilowania i uruchomienia gry potrzeba:
   - Kompilator C++
   - Zainstalowana biblioteka SFML (wersa 3.0 lub nowsza)
   - Narzędzia Git
2. Pobranie projektu
Sklonuj repozytorium na swój dysk lokalny:
git clone https://github.com/Kapionek/projekt_podstawy_informatyki.git
cd projekt_podstawy_informatyki
3. Kompilacja gry
Jeśli używasz kompilatora g++ i masz zainstalowaną bibliotekę SFML:
g++ *.cpp -o Zbijak -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
4. Uruchomienie gry
Gdy kompilacja się skończy wpisz:
./Zbijak

5. Sterowanie
Gra przeznaczona jest dla dwóch osób grających na jednej klawiaturze:
Gracz 1: Ruch: WSAD, Dash: E, Strzał: C
Gracz 2: Ruch: IKJL, Dash: O, Strzał: .

6. Funkcje gry:
- Fizyka: realityczne odbijanie piłek od ścian i innych obiektów
- Mechanika ruchu: płynne przyśpiszanie postaci oraz brak przenikania przez ściany
- Powerupy: Speedup - zwieksza prędkość gracza, Slowdown - spowalnia przeciwnika, Big Ball - zwiększa rozmiar wystrzeliwanej piłki, Multishot - pozwala na wystrzelenie dwóch piłek naraz
- Personalizacja: możliwość wpisywania nicków graczy przed rozpoczęciem rozgrywki
- Audio: dynamiczna muzyka z możliwością regulacji głośności w menu
