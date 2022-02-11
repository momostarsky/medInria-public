#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medAbstractDataSource.h>
#include <medDataIndex.h>

class medDatabaseDataSourcePrivate;

/**
* Not a classical data source per se, as it does not import data
* Still it fits to the design of data sources
*/
class medDatabaseDataSource : public medAbstractDataSource
{
    Q_OBJECT

public:
    medDatabaseDataSource(QWidget *parent = nullptr);
    ~medDatabaseDataSource();

    QWidget* mainViewWidget();
    QWidget* compactViewWidget();

    QWidget* sourceSelectorWidget();

    QString tabName();

    QList<medToolBox*> getToolBoxes();

    QString description() const;

public slots:
    void onOpeningFailed(const medDataIndex& index, QUuid);
    void currentSourceChanged(int current);

signals:
    void open(const medDataIndex&);
    void changeSource(int current);

protected slots:
    void onFilter(const QString &text, int column);
    void compactFilter(const QString &text, int column);

private:
    medDatabaseDataSourcePrivate* d;
};
