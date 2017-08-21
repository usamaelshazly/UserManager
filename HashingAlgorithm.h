/*
 * HashingAlgorithm.h
 *
 *  Created on: Aug 16, 2017
 *      Author: root
 */

#include "ui_HashingAlgorithm.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <sys/stat.h>
#include "myLibb.h"


using namespace std;

class HashingAlgorithm : public QDialog , public Ui::HashingAlgorithm
{

Q_OBJECT

public:
	HashingAlgorithm( QWidget *parent = 0 );
	~HashingAlgorithm();
	int setPassword(QString name);

};
