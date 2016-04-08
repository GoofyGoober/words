####ISTRUZIONI

- Fare doppio click sul file “**words**”.
- Si aprirà una finestra nera. Se si usa un monitor secondario o un proiettore, trascinare la finestra sulla schermata che dovrà visualizzare le parole.
- Premere il tasto “**f**” (fullscreen).
- La finestra si ingrandirà per occupare tutto lo spazio della schermata disponibile.
- Premere “**g**” (GUI = Graphic User Interface).
- La zona gialla che si vede rappresenta lo spazio entro cui verranno proiettate le parole. Nel caso ce ne fosse bisogno, si può scegliere di ridimensionare la zona gialla e di spostarla. Per farlo basta cambiare i parametri “**x**” e “**y**” sotto il pannello “**size**” della GUI e i parametri “**x**” e “**y**” sotto il parametro “**pos**” della GUI.
- Una volta decisa la grandezza della zona gialla (e quindi della zona entro cui dovranno apparire le parole), premere la lettera “**s**” (start);
- A questo punto la zona gialla dovrebbe essere scomparsa e dovrebbe aprirsi in automatico una finestra del browser con questo indirizzo: **http://localhost:9092/**.
- Se il browser che si è aperto non è **Google Chrome**, chiudere la finestra del browser che si è aperto, aprire **Google Chrome** e nella barra degli indirizzi copiare questo indirizzo **http://localhost:9092/** e premere invio.
- A questo punto quando si parla le parole dovrebbero apparire all’interno della schermata nera.


####NOTE SULLA GUI

- La gui appare e scompare premendo la lettera “**g**”.
- “**Scream for clean**” : se attivo, le parole si sfocano pian piano e tornano nitide man mano che si parla. Se disattivo le parole sfocano mentre si parla;
- “Speed Blur Disappear” : se attivo “Scream for clean”, regola la velocità della sfocatura;
- “**Life min minute**” e “**Life max minute**” servono per impostare la durata di ogni singola parola, prima di sparire. Il valore espresso è in minuti.
- “**Min Words Size Perc**” serve a impostare la larghezza minima che una parola può avere. La larghezza è in percentuale relativa alla larghezza dell’area in cui appariranno le parole;
- “**Max Words**” è il numero massimo di parole che ci possono essere sullo schermo. Se ci sono più parole di quanto stabilito, le parole in eccesso vengono fatte sparire più rapidamente;