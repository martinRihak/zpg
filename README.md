# ZPG projekt

- Pridat Observer a Subject jako samostatne tridy, ze kterych budou dědit - 1
- Vsechny FragmentShadery vytvořit
- Vytvorit tridy pro Transformace Scale, Rotation a Move
- - Nakreslit si diagram, jak bude transkofmace pristupovat k tridam a jak je bude skladat
- Podivat se na správné skládání Composite transformaci.
- Neposilat cas do vsech DrawableObject a vypocitavat cas primo v objektech (mozna teda)
- Zjednodušit správu a vytváření scén, nemusim vytvaret sceny, ale automaticky vytvorit scenu pokud scena neexistuje.
- Vytvorit Tridu Light a posilat podle pocize a intenzity informace do fragmentShaderu.
- 




## Discord popis

Ahoj, nepochopil jsem jak chce Němec tu Transform třídu a ty ostatní třídy Translate Rotate Scale
Chápu jak je to myšleno že se do vectoru nahodí jednotlivé transformace a potom ta composit transform třida to celé spočte a vrátí mat4
Ale výhody tohoto přístupu? + Co třeba myslí tou Dynamickou rotací. Kde se má updatovat pozice objektu, nebo vůbec kde vytvořit ten composit transform a nahrát do něj ty tranformace, atd.. 

Daniel KrásnýStudy Calls — včera v 16:58úterý 14. října 2025 v 16:58
Na přednášce říkal, že je výhodou právě to předpočítání, tedy že víš výsledek těch transformací dopředu a nemusíš to s každým snímkem počítat na GPU (ostatně proto používáme projektivní prostor). Ano, není to efektivní přístup (to násobení matic) v případě, že máš 1 trojúhelník a 1 transformaci, 1 trojúhelník a 1000 transformací, dokonce ani v případě 1000 trojúhelníků a 1 transformace. V tom případě se víc vyplatí si to radši vypočítat shaderem. Jakmile ale máš více trojúhelníků a aplikuješ na ně více transformací, může se výrazněji vyplatit právě varianta přednásobení modelové matice.

Transform třída má fungovat na návrhovém vzoru Composite, tedy vytvoříš jednu třídu, která bude mít abstraktní metodu, např. getModelMatrix(), budou jí implementovat potomci Translate, Rotate, Scale, ale také "kompozitní třída", jejíž obsahem bude rovněž implementace výše uvedené abstraktní metody, ale také vektor všech transformací a metodu pro přidání transformace do tohoto vektoru.

Dynamická rotace pak není nic jiného, než vrácení rotace (tzn. potomek třídy Rotate), akorát s tím rozdílem, že kdykoliv se zavolá ten getModelMatrix(), tak se provede změna v rotaci o předem stanovenou velikost, kterou určíš v konstruktoru. Jde o to, že každý snímek (každá iterace v hlavní smyčce OpenGL) při vykreslování bude do shaderu posílat aktuální modelovou matici, kterou si získá z té kompozitní třídy.

nejsem expert na ZPG a toto je čistě snaha kolegovi pomoct na základě informací, které se opakují na přednášce. pokud píšu nesmysly, tak se za ně omlouvám

^_^ — včera v 17:12úterý 14. října 2025 v 17:12
Díky za skvělou odpověď
Takže při vytvoření DrawableObject přidáme do Kompozitní třídy jednotlivé transformace, které se mají každý frame vykonat a popřípadě i transformace které jsou dynamické a mění svou hodnotu za běhu
Pokud bych chtěl změnit pozici toho objektu, jenom třeba jedenkrát posunout doprava. Musel bych v Kompozitní třídě najít danou transformaci pro translaci objektu a tam změnit hodnotu?

Díky za skvělou odpověď Takže při vytvoření DrawableObject přidáme do Kompozitní třídy jednotlivé transformace, které se mají každý frame vykonat a popřípadě i transformace které jsou dynamické a mění svou hodnotu za běhu Pokud bych chtěl změnit pozici toho objektu, jenom třeba jedenkrát posunout doprava. Musel bych v Kompozitní třídě najít danou transformaci pro translaci objektu a tam změnit hodnotu?

mod — včera v 17:20úterý 14. října 2025 v 17:20
Transformace se "nevykonávají", měly by to být prostě jenom hodnoty. Když chceš jen jednou změnit pozici toho objektu, tak ano, můžeš najít tu translaci která ti s objektem pohla, a upravit ji, a nebo můžeš přidat další translaci, která ti ten objekt opět posune. Tam samozřejmě záleží na tom, jak často to budeš posouvat, kdybys přidával novou translaci každý frame, tak si za chvilku zahltíš paměť


Daniel KrásnýStudy Calls — včera v 17:22úterý 14. října 2025 v 17:22
Možná to kolega jen špatně interpretoval, ale pohyb ve světě pak řeší kamera a projekční, resp. pohledová matice. Nepřijde mi, že budeme u projektu potřebovat na základě ovládání uživatele skutečně pohybovat s objekty.


