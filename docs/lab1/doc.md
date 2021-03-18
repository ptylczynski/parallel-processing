# Pomiar prędkości przetwarzania w funkcji użytej liczby wątków/procesorów dla różnych wersji kodu – obserwacja kosztów współdzielenia danych między wątkami. System komputera wielordzeniowego z pamięcią współdzieloną, środowisko Windows/Visual Studio lub Linux

## Wersja 1 - przetwarzanie sekwencyjne
- czas obliczeń: 109ms
- czas obliczeń (omp): 109ms

## Wersja 2 - proste zrównoleglenie
- lokalność:
  - zmienne prywatne: `i`
  - zmienne współdzielone: **wszystkie pozostałe**
  
Czas przetwarzania w wersji równoległej jest podobny do wersji szeregowej. Spodziewany (błędnie) wzrost prędkości przetwarzania nie nastąpił bez względu na ilość wątków logicznych. Przycznyą takiego zachowania jest wielokrotne uniważnianie liń pamięci w czasie przetwarzania ze względu na współdzielenie zmięnnych, np. `x`, albo `sum`. Ze względu na występujący wyścig w dostępie do zmiennej `sum` wynik przetwarzania jest niepoprawny.

## Wersja 3 - atomic
- lokalność:
  - bez zmian
  
Zastosowanie klauzuli `#pragma omp atomic` spowodowało, że obliczenia równoległe kończą się zwróceniem poprawnego wyniku. Niestety kosztem wymuszenia atomowości uaktualnienia wartośći zmiennej `sum` jest znaczne wydłużenie czasu wyknonania programu względem wersji sekwencyjnej. Głownym powodem jest potrzeba każdorazowego zakładania blokady na zmienną `sum`, co w przypadku proponowanego programu sprowadza go do wersji sekwencyjnej. Dalsze pogorszenie czasu przetwarzania wiążę się z potrzebą pobierania do pamięci cache procesora nowych wartości zmiennej `sum`, w większości przypadków, kiedy chcemy ją aktualizować. Zmienna jest współdzielona przez wszystkie wątki, a każdy wątek w każdym wykonaniu pętli ją aktualizuje, co powoduje unieważnienie lokalnych kopii tej zmiennej dla wątka/rdzenia/procesora.
