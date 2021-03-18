# Pomiar prędkości przetwarzania w funkcji użytej liczby wątków/procesorów dla różnych wersji kodu – obserwacja kosztów współdzielenia danych między wątkami. System komputera wielordzeniowego z pamięcią współdzieloną, środowisko Windows/Visual Studio lub Linux

## Specyfikacja
- ilość rdzeni: 4
- ilość wątków logicznych: 8
- Hyper-Threading: tak
- System Operacyjny: Windows 10 20H2

## Wersja 1 - przetwarzanie sekwencyjne
- czas obliczeń: 109ms
- czas obliczeń (omp): 109ms

## Wersja 2 - proste zrównoleglenie
- czas obliczeń:
  - 2 wątki: 
  - 4 wątki:
  - 8 wątków: 
- lokalność:
  - zmienne lokalne: `i`
  - zmienne współdzielone: *wszystkie pozostałe*
-  przyspieszenie:
   -  brak
  
Czas przetwarzania w wersji równoległej jest dłuższy względem wersji szeregowej. Spodziewany (błędnie) wzrost prędkości przetwarzania nie nastąpił bez względu na ilość wątków logicznych. Przycznyą takiego zachowania jest wielokrotne uniważnianie liń pamięci w czasie przetwarzania ze względu na współdzielenie zmięnnych, np. `x`, albo `sum`. Ze względu na występujący wyścig w dostępie do zmiennej `sum` wynik przetwarzania jest niepoprawny.

## Wersja 3 - atomic
- czas obliczeń:
  - 2 wątki: 
  - 4 wątki:
  - 8 wątków: 
- lokalność:
  - bez zmian
-  przyspieszenie:
   -  brak
  
Zastosowanie klauzuli `#pragma omp atomic` spowodowało, że obliczenia równoległe kończą się zwróceniem poprawnego wyniku. Niestety kosztem wymuszenia atomowości uaktualnienia wartośći zmiennej `sum` jest znaczne wydłużenie czasu wyknonania programu względem wersji sekwencyjnej. Głownym powodem jest potrzeba każdorazowego zakładania blokady na zmienną `sum`, co w przypadku proponowanego programu sprowadza go do wersji sekwencyjnej. Dalsze pogorszenie czasu przetwarzania wiążę się z potrzebą pobierania do pamięci cache procesora nowych wartości zmiennej `sum`, w większości przypadków, kiedy chcemy ją aktualizować. Zmienna jest współdzielona przez wszystkie wątki, a każdy wątek w każdym wykonaniu pętli ją aktualizuje, co powoduje unieważnienie lokalnych kopii tej zmiennej dla wątka/rdzenia/procesora.

## Wersja 4 - lokalne zmienne
- czas obliczeń:
  - 2 wątki: 
  - 4 wątki:
  - 8 wątków: 
- lokalność:
  - zmienne lokalne: `i`, `suml`, `x`
  - zmienne współdzielone: *wszystkie pozostałe*
-  przyspieszenie:
   -  tak
  
W wyniku "lokalizacji" zmiennych czas przetwarzania równoległego, poraz pierwszy okazał się krótszy od czas przetwarzania sekwencyjnego. Dodanie zmiennych lokalnych spowodowało, że nie ma już potrzby każdorazowego uniważniania liń pamięci. Od teraz może to wystąpić tylko w momencie dodawania lokalnej sumy do sumy globalnej, jednak takich dodawań jest znacznie mniej niż wcześniej, co przyczyniło się do wrostu tempa przetwarzania.

## Wersja 5 - redukcja
Jedyna zmiana zachodząca w kodzie to dodanie nowej części `reduction(+:sum)` do istniejącej dyrektywy `#pragma omp parallel for`. Nie powoduje to istotnych zmian w generowanym kodzie, dlatego wszystkie wyniki z Wersji 4 pozostają aktualne.

## Wersja 6 - tablica
- czas obliczeń:
  - 2 wątki: 
  - 4 wątki:
  - 8 wątków: 
- lokalność:
  - zmienne lokalne: `i`, `x`
  - zmienne współdzielone: *wszystkie pozostałe*
-  przyspieszenie:
   -  tak

Zastosowanie tablicy do przechowywania obliczeń wykonywanych w poszczególnych wątkach skraca czas wykonywania programu równoległego. Należy jednak pamiętać o dobraniu odpowiedzniego rozmiaru tablicy, oraz poprawności jej wykorzystania. Poszczególne wątki nie mogą pracować na adresach tablicy znajdujących się za blisko - nie bliżej niż długość lini adresowej dzielonej przez wielkość danej. Jeżeli nie zapewnimy odpowiedniej odległości to wątek aktualizując swoją zmienną w tablicy może "najechać" na zmienną innego wątku - uniważnić jego dane - zapis. Dzieje się tak ponieważ zmienna innego wątku znajdzie się na lini adresowej wątku wykonującego aktualizację. 