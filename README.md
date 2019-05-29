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

### Special effects
Special effects umožňuje kopírovat a přesouvat veškerá nastavení mezi LED diodami. Navigace opět probíhá pomocí GB.

### Ethernet settings
V Ethernet settings uživatel zapne ethernetové spojení. Na místní síti pak řídící jednotka najde jinou řídící jednotku s programem KOREK a může ovládat její LED diody.

### KOREK settings
Celá složka menu "KOREK settings" je ovládáná výhradně pomocí GB.
Postupně umožňuje tyto volby: invertování barev menu ("Invert color of GUI"), změna velikosti písma GUI - 16px vs 32px ("Change text size"), uložení aktuálního nastavení LED diod ("Save LEDs settings"), načtení posledního uloženého nastavení LED diod ("Load LEDs settings").

### Exit KOREK
Volbou složky "Exit KOREK" a potvrzením položky "Confirm exit" je ukončen program KOREK.

## Dokumentace Hardware
MICROZED EVALUATION KIT.
ADSAES-Z7MB-7Z010-G.
Xylinx Zynq 7Z010.
Základní čip: Xilinx Zynq-7000 All Programmable SoC.
Typ: Z-7010, součástka XC7Z010.
CPU: Dual ARM® Cortex™-A9 MPCore™ @ 866 MHz (NEON™ & Single / Double Precision Floating Point).
2x L1 32 kB data +32 kB instruction, L2 512 KB.
FPGA: 28K Logic Cells (~430K ASIC logic gates, 35 kbit).
Výpočetní jednotky v FPGA: 100 GMACs.
Paměti v FPGA: 240 KB.
Paměť na desce MicroZed: 1GB.
Operační systém: GNU/Linux.
GNU LIBC (libc6) 2.19-18+deb8u7.
Jádro Linux 4.9.9-rt6-00002-ge6c7d1c.
Distribuce: Debian Jessie.

1G ETHERNET.
USB Host, konektor A.
Sériový port UART1 přes převodník na USB, USB mikro-B.
Mikro SD karta.
Uživatelské tlačítko a reset tlačítko.

Více na: http://microzed.org/product/microzed .

