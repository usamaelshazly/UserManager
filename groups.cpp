#include "groups.h"
#include "myLibb.h"
/**
 Constructor κλάσης 
 */
Groups::Groups( )
{} 
/**
 *Destructor κλάσης
 */
Groups::~Groups( )
{}
/**
 *Εισάγει κάθε νέο χρήστη σε σε βασικές ομάδες όπως audio (ήχο),video (χρήση βίντεο),cdrom (χρήση cdrom),floppy (χρήση δισκέτας) και plugdev (χρήση αποθηκευτικών μέσων).
 */
int Groups::default_groups(QString newmem)
{
struct group *grs;
setgrent();
QString allmem = "";
while((grs = getgrent()))
{
if (strcmp(grs->gr_name,"video")==0 || strcmp(grs->gr_name ,"cdrom")==0 || strcmp(grs->gr_name,"floppy")==0 || strcmp(grs->gr_name ,"audio")==0 ||  strcmp(grs->gr_name, "plugdev")==0 )
{
QString member =  *grs->gr_mem;
if(*grs->gr_mem == NULL)
*grs->gr_mem = newmem.toAscii().data();
else
{
allmem = "" + member + ","  + newmem + "";
*grs->gr_mem = allmem.toAscii().data();
}
MyLibb set;
if(set.setgrnam(grs) < 0)return -1;
allmem = "";
}
}
endgrent();
return 0;
}
/**
 *Αναζήτηση μιας ομάδας στο σύστημα.Ψάχνει στο αρχείο των ομάδων και αν βρεθεί η ομάδα που αναζητούμε 
επιστρέφει την δομή της.Αν όχι εμφανίζει ανάλογο μήνυμα.
 */
struct group *Groups::searchGroup()
{
	bool okBtn;
	char *groupToSearch;
	struct group *result = NULL;
	QString groupString = QInputDialog::getText( 0, QObject::tr( "Search" ), QObject::tr( "Enter Group Name:(case sensitive)" ), QLineEdit::Normal, QString( "" ), &okBtn );

	if ( okBtn && groupString != " " )
	{
		groupToSearch = groupString.toAscii().data();
		result = getgrnam( groupToSearch );
		if ( result == NULL )
		{
			QMessageBox::information( 0, QObject::tr( "User Manager" ), QObject::tr( "Group %1 not found" ).arg( groupString ) ) ;
		}

	}
	return result;
}
/**
 *Διαγράφει ένα μέλος από μια ομάδα.Παίρνει την δομή της ομάδας με τα μέλη της και αν βρεί τον χρήστη που
θέλουμε να αφαιρέσουμε από την ομάδα (όρισμα uname),αντικαθιστά τη θέση του στα μέλη με ένα κενό.Στην συνέχεια καθαρίζει το αρχείο των ομάδων από πιθανά άχρηστα κόμματα που έμειναν μέσω του κειμενογράφου sed.
Στο τέλος επιστρέφει την δομή χωρίς το μέλος που αφαιρέθηκε.
 */
struct group *Groups::remove_member(struct group *in,char *uname)
{	
	int i;
	QString memo = "";
	if (in == NULL) return NULL;
	
	for (i = 0; in->gr_mem[i]; i++)
		{
			if (!strcmp(in->gr_mem[i], uname))
			{
				in->gr_mem[i] = memo.toAscii().data();
			}
		}
	system("sed -i 's/,,/,/g;s/,$//g' /etc/group");//['s/,,/,/g]->antika8ista ta dyo kommata me ena. [s/,$//g']->antika8ista to koma sto telos ths ka8e grammhs me keno
	return in;
}

/**
 *Διαγραφή ενός χρήστη από όλες τις ομάδες που πιθανόν να ήταν μέλος.Χρησιμοποιείται αφού διαγραφεί κάποιος χρήστης από το σύστημα.
 */

int Groups::rm_grMember(QString log)
{
    struct group *grs;
	FILE * f;
	MyLibb *set;
	f = fopen(GR_FILE,"r");
	char *login = log.toAscii().data();
	setgrent();
	while((grs = fgetgrent(f)))
	{		
		remove_member(grs , login);
		if(set->setgrnam(grs)<0)
		{
			return -1;
		}
	}
	endgrent();
	fclose(f);
	return 0;
}
/**
 *Προετοιμασία διαγραφής μια ομάδας από το σύστημα. 
 */
int Groups::deleteGroup(QString gname)
{
	MyLibb set;
	struct group *gr = NULL;
	char * groupname = gname.toAscii().data();
	if(set.setgrnam_r(gr,groupname)<0)
		{
			delete groupname;
			return -1;
		}
	delete groupname;
	return 0;
}
/**
 *Ελέγχει αν υπάρχει χρήστης ο οποίος έχει σαν κύρια ομάδα,την ομάδα με το GROUP ID που παίρνει σαν όρισμα.
 */
bool Groups::isPrimary(int gid)//h synarthsh epistrefei true an yparxei kapoios xrhsths pou exei to group san prwteywn
{
gid_t gidt=gid;

bool found=false;
struct passwd *users;
setpwent();
while (( users = getpwent() ) )
{
	if (users->pw_gid==gidt)
	{
			found=true;
	}
}
endpwent();
return found;

}
/**
 *Δημιουργία ενός string το οποίο περιέχει όλους τους χρήστες οι οποίοι έχουν σαν κύρια ομάδα την ομάδα με το GROUP ID που παίρνει σαν όρισμα.
 */
QString Groups::getPrimaryUsers(int gid) // epistrefei tous xrhstes pou exoun to group san prwteywn
{
gid_t gidt = gid;
QString primOfUsers;
struct passwd *users;
setpwent();
while (( users = getpwent() ) )
{
	if (users->pw_gid==gidt)
	{
		primOfUsers.append(users->pw_name);
		primOfUsers.append(",");
	}
}
endpwent();
primOfUsers.chop(1);//kovei 1 xarakthra ap to telos(to teleytaio komma)
return primOfUsers;
}
