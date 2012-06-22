/*
Copyright (c) 2011 BitTorrent, Inc. All rights reserved.

Use of this source code is governed by a BSD-style that can be
found in the LICENSE file.
*/

var LANG_STR = [
	  "Αρχεία Torrent||*.torrent||Όλα τα αρχεία (*.*)||*.*||"
	, "OK"
	, "Άκυρο"
	, "Εφαρμογή"
	, "Ναι"
	, "Όχι"
	, "Κλείσιμο"
	, "Επιλογές"
	, "Γλώσσα"
	, "Γλώσσα:"
	, "Ιδιωτικό απόρρητο"
	, "Έλεγχος για νέες εκδόσεις αυτόματα"
	, "Δοκιμαστικές εκδόσεις"
	, "Αποστολή ανώνυμων πληροφοριών κατά τον έλεγχο για αναβάθμιση"
	, "Κατα τη λήψη"
	, "Κατάληξη .!ut σε μη ολοκληρωμένα"
	, "Δέσμευση χώρου για τα αρχεία"
	, "Αποτροπή αδρανοποίησης με torrents ενεργά"
	, "Επιλογές προβολής"
	, "Επιβεβαίωση διαγραφής torrents"
	, "Επιβεβαίωση διαγραφής trackers"
	, "Ερώτημα επιβεβαίωσης για έξοδο"
	, "Αλλαγή χρώματος φόντου λίστας"
	, "Τρέχουσα ταχύτητα σε γρ. τίτλου"
	, "Όρια ταχύτητας σε γρ. κατάστασης"
	, "Όταν προστίθενται Torrents"
	, "Μη αυτόματη έναρξη της λήψης"
	, "Ανάδυση παραθύρου προγράμματος"
	, "Προβολή παραθύρου που δείχνει τα αρχεία μέσα στο torrent"
	, "Ενέργεια διπλού κλικ"
	, "Για torrents που τροφοδοτούν:"
	, "Για torrents που λαμβάνονται:"
	, "Τοποθεσία ολοκληρωμένων αρχείων"
	, "Αποθήκευση νέων λήψεων:"
	, "Παράθυρο διαλόγου για ορισμό"
	, "Μεταφορά των ολοκληρωμένων στο:"
	, "Επισύναψη ετικέτας του torrent"
	, "Μεταφορά μόνο απο τον προκαθορισμένο φάκελο λήψεων"
	, "Τοποθεσία των .torrents"
	, "Αποθήκευση νέων .torrents στο:"
	, "Μεταφορά ολοκληρωμένων .torrents στο:"
	, "Αυτόματη φόρτωση .torrents από:"
	, "Διαγραφή ενεργών .torrents"
	, "Θύρα επικοινωνίας"
	, "Θύρα για τις εισερχόμενες συνδέσεις:"
	, "Τυχαία θύρα"
	, "Τυχαία θύρα σε κάθε εκκίνηση"
	, "Ενεργοποίηση του UPnP"
	, "Ενεργοποίηση του NAT-PMP"
	, "Διακομιστής μεσολάβησης"
	, "Τύπος"
	, "Διεύθυν."
	, "Θύρα"
	, "Ταυτοποίηση"
	, "Όνομα χρήστη:"
	, "Κωδικός πρόσβασης:"
	, "Να γίνονται Resolve τα hostnames μέσω του διαμεσολαβητή"
	, "Χρήση του διαμεσολαβητή για συνδέσεις peer-to-peer"
	, "Εξαίρεση από το Windows Firewall"
	, "Ιδιωτικότητα διαμεσολαβητή"
	, "Απενεργοποίηση όλων των τοπικών DNS lookups"
	, "Απενεργοποίηση δυνατοτήτων που ενδέχεται να παρέχουν πληροφορίες αναγνώρισης"
	, "Απενεργοποίηση συνδέσεων που δεν υποστηρίζονται απ τον διαμεσολαβητή"
	, "Γενικό όριο ταχύτητας αποστολής"
	, "Μέγιστη ταχύτητα αποστολής (kB/s): [0: απεριόριστο]"
	, "Αυτόματα"
	, "Μέγιστο όριο αποστολής όταν δεν γίνεται λήψη (kB/s):"
	, "Γενικό όριο ταχύτητας λήψεων"
	, "Μέγιστη ταχύτητα λήψης (kB/s):"
	, "Αριθμός συνδέσεων"
	, "Γενικό όριο αριθμού συνδέσεων:"
	, "Μέγιστος αριθμός συνδεδεμένων αποδεκτών ανά torrent:"
	, "Μέγιστος αριθμός θέσεων αποστολής ανα torrent:"
	, "Χρήση επιπλέον θέσεων αποστολής αν η ταχύτητα αποστολής είναι < 90%"
	, "Global Rate Limit Options"
	, "Εφαρμογή ορίου ρυθμού στο transport overhead"
	, "Εφαρμογή ορίου ρυθμού στις συνδέσεις uTP"
	, "Βασικές λειτουργίες του BitTorrent"
	, "Ενεργοποίηση του δικτύου DHT"
	, "Πληροφορίες από tracker για scrape"
	, "Ενεργοποίηση DHT για νέα torrents"
	, "Ενεργοπ. ανταλλαγής αποδεκτών"
	, "Αποκάλυψη τοπικών αποδεκτών"
	, "Όριο εύρους ζώνης τοπικών αποδεκ."
	, "Αναφερόμενο IP/Hostname σε tracker:"
	, "Κρυπτογράφηση προτοκόλλου"
	, "Εξερχόμενες:"
	, "Αποδοχή απλών εισερχ. συνδέσεων"
	, "Ενεργοποίηση διαχείρισης bandwidth [uTP]"
	, "Ενεργοποίηση υποστήριξης UDP tracker"
	, "Ενεργοποίηση ορίου μεταφόρας"
	, "Ρυθμήσεις όριων"
	, "Τύπος ορίου:"
	, "Όριο εύρους:"
	, "Χρονική περίοδος (μέρες):"
	, "Ιστορικό χρήσης της επιλεγμένης περιόδου:"
	, "Uploaded:"
	, "Downloaded:"
	, "Uploaded + Downloaded:"
	, "Περίοδος:"
	, "Τελευταίες %d μέρες"
	, "Καθαρισμός ιστορικού"
	, "Ρυθμίσεις προτεραιότητας"
	, "Μέγιστος αριθμός ενεργών torrents: (αποστολή ή λήψη):"
	, "Μέγιστος αριθμός ενεργών λήψεων:"
	, "Να γίνεται τροφοδοσία όταν [εξ'ορισμού]"
	, "Ελάχιστο ratio (%):"
	, "Ελάχιστος χρόνος διαμοιρασμού (λεπτά):"
	, "Οι εργασίες αποστολής έχουν μεγαλύτερη προτεραιότητα απο τη λήψη"
	, "Όταν το µTorrent πετυχαίνει το στόχο τροφοδοσίας"
	, "Περιορισμός της ταχύτητας αποστολής σε (kB/s):"
	, "Ενεργοποίηση χρονοπρογραμματιστή"
	, "Πεδίο χρονοπρογραμματισμού"
	, "Ρυθμίσεις χρονοπρογραμματισμού"
	, "Περιορισμένη ταχύτητα αποστ. (kB/s):"
	, "Περιορισμένη ταχύτητα λήψης (kB/s):"
	, "Απενεργοποήση DHT σε διακοπή"
	, "Ενέργοποίηση του Web UI"
	, "Ταυτοποίηση"
	, "Όνομα χρ:"
	, "Κωδικός Πρ:"
	, "Ενεργοποίηση του λογαριασμού Guest με όνομα:"
	, "Συνδεσιμότητα"
	, "Εναλλακτική θύρα σύνδεσης (της θύρας επικοινωνίας):"
	, "Περιορισμός πρόσβασης στις ακόλουθες IPs (Διαχωρίστε τις εισαγωγές με κόμμα):"
	, "Προχωριμένες ρυθμίσεις (ΠΡΟΣΟΧΗ: Μην τις αλλάξετε!)"
	, "Τιμή:"
	, "Αληθής"
	, "Ψευδής"
	, "Ορισμός"
	, "Αναδυόμενη λίστα ταχυτήτων (Διαχωρισμός τιμών με κόμμα)"
	, "Παράβλεψη αναδυόμενης λίστας ταχυτήτων"
	, "Λίστα ταχυτ. αποστ.:"
	, "Λίστα ταχυτ. λήψης:"
	, "Μόνιμες ετικέτες (Διαχωρισμός ετικετών με το χαρακτήρα | )"
	, "Μηχανές αναζήτησης (Μορφή: όνομα|URL)"
	, "Βασικές ρυθμίσεις της Cache"
	, "Η cache δίσκων χρησιμοποιείται για να κρατήσει τα συχνά χρησιμοποιούμενα στοιχεία στη μνήμη για να μειώσει τον αριθμό αναγνώσεων/εγγραφών στο σκληρό δίσκο. Το µTorrent κανονικά διαχειρίζεται την cache αυτόματα, αλλά μπορείτε να αλλάξετε τη συμπεριφορά της με την τροποποίηση αυτών των ρυθμίσεων."
	, "Χρήση του παρακάτω μεγέθους cache (ΜΒ):"
	, "Χρήση λιγότερης μνήμης όταν η cache δεν χρειάζεται"
	, "Προχωρημένες ρυθμίσεις της Cache"
	, "Ενεργοποίηση της cache για τις εγγραφές στο σκληρό δίσκο"
	, "Εγγραφή απείραχτων κομματιών κάθε 2 λεπτά"
	, "Άμεση εγγραφή των ολοκληρωμένων κομματιών"
	, "Ενεργοποίηση της cache για τις αναγνώσεις"
	, "Κλείσιμο της cache ανάγνωσης όταν η ταχύτητα αποστολής είναι χαμηλή"
	, "Αφαίρεση παλαιών κομματιών απ την cache"
	, "Αυτόματη αύξηση μεγέθους της cache όταν χρειάζεται"
	, "Απενεργοποίηση λειτουργίας cache των Windows για τις εγγραφές στο δίσκο"
	, "Απενεργοποίηση λειτουργίας cache των Windows για τις αναγνώσεις στο δίσκο"
	, "Εκτέλεση προγράμματος"
	, "Εκτέλεση αυτού του προγράμματος όταν ένα torrent ολοκληρωθεί:"
	, "Εκτέλεση αυτού του προγράμματος όταν ένα torrent αλλάζει κατάσταση:"
	, "Μπορείτε να κάνετε χρήση των εντολών:\r\n%F - Όνομα κατεβασμένου αρχείου (για μεμομονένα torrents)\r\n%D - Φάκελος αποθήκευσης αρχείων\r\n%N - Τίτλος του torrent\r\n%S - Κατάσταση του torrent\r\n%L - Ετικέτα\r\n%T - Tracker\r\n%M - String μηνύματος κατάστασης (ίδιο με την στήλη κατάστασης)\r\n%I - δεκαδεξαδική κωδικοποίηση info-hash\r\n\r\nΗ κατάσταση είναι συνδυασμός από:\r\nΞεκίνησε = 1, έλεγχος = 2, έναρξη-από-έλεγχο = 4,\r\nελέγχθηκε = 8, σφάλμα = 16, παύθηκε = 32, αυτόματο = 64, φορτώθηκε = 128"
	, "Ιδιότητες Torrent"
	, "Trackers (Διαχωρισμός με μια κενή γραμμή)"
	, "Ρυθμήσεις εύρους ζώνης"
	, "Μέγιστη ταχύτητα αποστολής (kB/s): [0: προεπιλογή]"
	, "Μέγιστη ταχύτητα λήψης (kB/s): [0: προεπιλογή]"
	, "Αριθμός θέσεων αποστολής: [0: προεπιλογή]"
	, "Τροφοδοσία ενώ"
	, "Αγνόηση των προεπιλεγμένων ρυθμίσεων"
	, "Ελάχιστο ratio (%):"
	, "Ελάχιστος χρόνος διαμοιρασμού (λεπτά):"
	, "Άλλες ρυθμίσεις"
	, "Αρχική τροφοδοσία"
	, "Ενεργοποίηση DHT"
	, "Ανταλλαγή αποδεκτών"
	, "Τροφοδοσία"
	, "URL Τροφοδοσίας"
	, "Προσαρμοσμένο Alias:"
	, "Συνδρομή"
	, "Να μη κατεβαίνουν αυτόματα όλα τα αντικείμενα"
	, "Αυτόματο κατέβασμα όλων των αντικειμένων στο FEED."
	, "Χρήση έξυπνου φίλτρου επεισοδίων"
	, "Τροφοδοσίες||Αγαπημένα||Ιστορικό||"
	, "All Feeds"
	, "Ρυθμίσεις φίλτρων"
	, "Όνομα:"
	, "Φίλτρο:"
	, "Όχι:"
	, "Αποθήκ:"
	, "Τροφ:"
	, "Ποιότητα:"
	, "Αριθμός Επεισοδίου: [πχ.1x12-14]"
	, "Tαίριασμα φίλτρου στο κανονικό όνομα αντί του αποκωδ."
	, "Μη αυτόματη έναρξη των λήψεων"
	, "Έξυπνο φίλτρο επ."
	, "Μέγιστη προτεραιότητα στην λήψη"
	, "Ελάχιστο διάστημα:"
	, "Ετικέτα νέων torrents:"
	, "Προσθήκη τροφοδοσίας RSS..."
	, "Επεξεργασία Τροφοδοσίας..."
	, "Απενεργοποίηση Τροφοδοσίας"
	, "Ενεργοποίηση Τροφοδοσίας"
	, "Ανανέωση Τροφοδοσίας"
	, "Διαγραφή Τροφοδοσίας"
	, "Λήψη"
	, "Άνοιγμα URL στο πρόγραμμα περιήγησης"
	, "Προσθήκη στα αγαπημένα"
	, "Προσθήκη"
	, "Διαγραφή"
	, "ΟΛΑ"
	, "(Όλα)"
	, "(πάντα ταίριασμα)||(ταίριασμα μια μόνο φορά)||12 ώρες||1 μέρα||2 μέρες||3 μέρες||4 μέρες||1 εβδομάδα||2 εβδομάσες||3 εβδομάδες||1 μήνας||"
	, "Προσθήκη RSS Feed"
	, "Επεξεργασία Τροφοδοσίας RSS"
	, "Remove RSS Feed(s)"
	, "Really delete the %d selected RSS Feeds?"
	, "Σίγουρα να διαγραφεί το RSS Feed \"%s\"?"
	, "Πλήρες Όνομα"
	, "Όνομα"
	, "Επεισόδιο"
	, "Μορφή"
	, "Κωδικοποιητής"
	, "Date"
	, "Feed"
	, "URL πηγής"
	, "IP"
	, "Θύρα"
	, "Πρόγραμμα"
	, "Σημαίες"
	, "%"
	, "Σχετικότητα"
	, "Ταχ. Λήψης"
	, "Ταχ. Αποστολής"
	, "Αιτ."
	, "Αναμονή"
	, "Απεσταλμένα"
	, "Ελήφθησαν"
	, "Hasherr"
	, "Ρυθμός Λ."
	, "Μεγ.Ταχ.Αποστολής"
	, "Μεγ.Ταχ.Λήψης"
	, "Αναμονή"
	, "Ανενεργό"
	, "Ολοκληρώθηκε"
	, "Πρώτο Κομμάτι"
	, "Όνομα"
	, "# Κομμάτια"
	, "%"
	, "Προτεραιότητα"
	, "Μέγεθος"
	, "παράβλεψη"
	, "χαμηλό"
	, "κανονικό"
	, "υψηλό"
	, "Λήφθηκαν:"
	, "Αποστάλθηκαν:"
	, "Seeds:"
	, "Απομένουν:"
	, "Ταχύτητα λήψης:"
	, "Ταχ. αποστολής:"
	, "Αποδέκτες:"
	, "Αναλογία Α/Λ:"
	, "Αποθήκευση ως:"
	, "Hash:"
	, "Γενικά"
	, "Μεταφορά"
	, "%d of %d συνδέθηκαν (%d in swarm)"
	, "Λ:%s Α:%s - %s"
	, "Αντιγραφή"
	, "Επαναφορά"
	, "Απεριόριστο"
	, "Ανάλυση των IP"
	, "Get File(s)"
	, "Μην κατεβάσεις"
	, "Υψηλή Προτεραιότητα"
	, "Χαμηλή Προτεραιότητα"
	, "Κανονική Προτεραιότητα"
	, "Αντιγραφή Magnet URI"
	, "Διαγραφή Δεδομένων"
	, "Διαγραφή .torrent"
	, "Διαγραφή .torrent + Δεδομένων"
	, "Εξαναγκασμένος Επανέλεγχος"
	, "Εξαναγκασμένη Εκκίνηση"
	, "Ετικέτα"
	, "Παύση"
	, "Επιλογές"
	, "Μετακίνηση κάτω στη σειρά"
	, "Μετακίνηση πάνω στη σειρά"
	, "Διαγραφή"
	, "Αφαίρεση και"
	, "Εκκίνηση"
	, "Διακοπή"
	, "Ενεργό"
	, "Όλα"
	, "Ολοκληρώθηκε"
	, "Γίνεται λήψη"
	, "Ανενεργό"
	, "Χωρίς ετικέτα"
	, "||Υγεία||Διαθεσιμότητα"
	, "Προστέθηκε στις"
	, "Ολοκληρώθηκε στις"
	, "Ολοκληρώθηκε"
	, "Ελήφθησαν"
	, "Ταχ. Λήψης"
	, "Περαίωση σε"
	, "Ετικέτα"
	, "Όνομα"
	, "#"
	, "Αποδέκτες"
	, "Απομένουν"
	, "Τροφοδότες"
	, "Τροφοδότες/Αποδέκτες"
	, "Αναλογία Α/Λ"
	, "Μέγεθος"
	, "URL πηγής"
	, "Κατάσταση"
	, "Εστάλησαν"
	, "Ταχ. Αποστολής"
	, "Είστε βέβαιοι οτι θέλετε να αφαιρέσετε τα %d επιλεγμένα torrents και όλα τα σχετικά με αυτά στοιχεία;"
	, "Είστε βέβαιοι οτι θέλετε να αφαιρέσετε το επιλεγμένο torrent και όλα τα σχετικά με αυτό στοιχεία;"
	, "Είστε βέβαιοι οτι θέλετε να αφαιρέσετε τα %d επιλεγμένα torrents?"
	, "Είστε βέβαιοι οτι θέλετε να αφαιρέσετε το επιλεγμένο torrent?"
	, "Σίγουρα να διαγραφεί το Φίλτρο RSS  \"%s\";"
	, "Ελέγχθηκε %:.1d%%"
	, "Γίνεται λήψη"
	, "Σφάλμα: %s"
	, "Ολοκληρώθηκε"
	, "Παύση"
	, "Αναμονή"
	, "Αναμονή Τροφοδοσίας"
	, "Τροφοδοσία"
	, "Σταματημένο"
	, "Εισαγωγή ετικέτας"
	, "Εισαγωγή νέας ετικέτας για τα επιλεγμένα torrents:"
	, "Νέα Ετικέτα..."
	, "Αφαίρεση Ετικέτας"
	, "Γενικά||Trackers||Αποδέκτες||Κομμάτια||Αρχεία||Ταχύτητα||Καταγραφέας||"
	, "Πρόσθήκη Torrent"
	, "Προσθήκη Torrent από URL"
	, "Παύση"
	, "Ρυθμίσεις"
	, "Μετακίνηση Κάτω στη σειρά"
	, "Μετακίνηση Πάνω στη σειρά"
	, "Διαγραφή"
	, "Λήψεις RSS"
	, "Εκκίνηση"
	, "Διακοπή"
	, "Αρχείο"
	, "Προσθήκη Torrent..."
	, "Προσθήκη torrent από URL..."
	, "Επιλογές"
	, "Επιλογές"
	, "Εμφάνιση Λίστας Κατηγοριών"
	, "Εμφάνιση λεπτομερών πληροφοριών"
	, "Εμφάνιση  γραμμή κατάστασης"
	, "Εμφάνιση γραμμής εργαλείων"
	, "Εικονίδια στις καρτέλες"
	, "Βοήθεια"
	, "Ιστοσελίδα μTorrent"
	, "Κοινότητα του µTorrent"
	, "Send WebUI Feedback"
	, "About µTorrent WebUI"
	, "Torrents"
	, "Παύση όλων των torrents."
	, "Συνέχιση όλων των torrents"
	, "D: %s%z/s"
	, " L: %z/s"
	, " O: %z/s"
	, " T: %Z"
	, "U: %s%z/s"
	, "B"
	, "EB"
	, "GB"
	, "kB"
	, "MB"
	, "PB"
	, "TB"
	, "Προχωρημένες"
	, "Εύρος ζώνης"
	, "Σύνδεση"
	, "Cache Δίσκου"
	, "Φάκελοι"
	, "Γενικά"
	, "Χρονοδιακόπτης"
	, "Προτεραιότητα"
	, "UI Έξτρα"
	, "Ρυθμίσεις UI"
	, "BitTorrent"
	, "Web UI"
	, "Όριο μεταφοράς"
	, "Εκτέλεση προγράμματος"
	, "Εμφάνιση Ιδιοτήτων||Έναρξη/Διακοπή||Άνοιγμα Φακέλου||Εμφάνιση μπάρας Λήψης||"
	, "Απενεργοποιημένο||Ενεργοποιημένο||Εξαναγκασμένο||"
	, "(κανένα)||Socks4||Socks5||HTTPS||HTTP||"
	, "Uploads||Downloads||Uploads + Downloads||"
	, "MB||GB||"
	, "1||2||5||7||10||14||15||20||21||28||30||31||"
	, "Όνομα"
	, "Τιμή"
	, "Δευ||Τρι||Τετ||Πεμ||Παρ||Σαβ||Κυρ||"
	, "Δευτέρα||Τρίτη||Τετάρτη||Πέμπτη||Παρασκευή||Σάββατο||Κυριακή||"
	, "Πλήρες"
	, "Μέγιστη ταχύτητα - Χρήση κανονικού γενικού όρια bandwidth"
	, "Ορισμένο"
	, "Περιορισμένη - Χρήση ρυθμίσεων προγραμματιστή Όρια bandwidth"
	, "Μόνο τροφοδοσία"
	, "Μόνο διαμοιρασμός - Μόνο ανέβασμα δεδομένων (τα ατελή αρχεία συμπεριλαμβάνονται)"
	, "Κλειστό"
	, "Διακοπή - Σταματάει όλα τα torrents που δεν είναι εξαναγκασμένα"
	, "<= %d ώρες"
	, "(αγνόησε)"
	, "<= %d λεπτά"
	, "%dd %dh"
	, "%dh %dm"
	, "%dm %ds"
	, "%ds"
	, "%dw %dd"
	, "%dy %dw"
];
