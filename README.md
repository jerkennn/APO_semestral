# APO Semestral project
# Kozel - Rektoris (KOREK)

Aplikace KOREK realizuje Distribuované řízení osvětlení (zjednodušeno na dvě RGB LED diody) prostřednictvím řídicí jednotky Microzed. Ovládací pult obsahuje tři otočné ovladače (buttons) s možností stisknutí - levý červený (dále RB), prostřední zelený (dále GB) a pravý modrý (dále BB).

## Orientace v textové menu, v němž lze volit jednotlivé funkce.
Po spuštění desky display zobrazí domovský adresář menu (dále Home). Pomocí otáčení GB lze vybírat jednotlivé složky menu a stisknutím GB se výběr potvrdí. Jedná se o:
1) základní nastavení LED diod - "Change LEDs"
2) pokročilé nastavení LED diod - "Special effects"
3) nastavení síťové komunikace - "Ethernet settings"
4) vlastní nastavení aplikace - "KOREK settings"
5) ukončení programu - "Exit KOREK"

### Change LEDs
GB opět naviguje v menu.
První tři položky mění složky HSV pro LED diody. Po potvrzení se v dolní části displaye objeví dva barevné pruhy s posuvníky (aktuální volba). Horní pruh náleží LED1 (výběr pomocí RB), dolní LED2 (výběr pomocí BB). V rámci potvrzení jedné z těchto tří položek je dále v menu možné staticky ovládat LED1, LED2, nebo zapnou animaci barev LED diod, kdy barevný rozsah je určen v položce "Change hue (H)", kdy posuvníky na barevných páscích určují meze barev, mezi kterými se mění animace.
Speciální položkou složky "Change LEDs" je položka "Change period", ve které si uživatel může navolit pro každou LED diodu periodu svícení "ON-Period" (ovládání točením RB), periodu zhasnutí "OFF-Period" (ovládání točením GB) a periodu přechodu animace barev "Anime-Period" (ovládání točením BB).
Do Home se uživatel vrátí výběrem položky "Back" a potvrzením stisknutím GB.
