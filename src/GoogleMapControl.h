/*
 * Copyright (c) 2009 Greg Lonnon (greg.lonnon@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _GC_GoogleMapControl_h
#define _GC_GoogleMapControl_h
#include "GoldenCheetah.h"

#include <QWidget>
#include <QtWebKit>
#include <string>
#include <iostream>
#include <sstream>
#include <string>
#include "RideFile.h"
#include "MainWindow.h"

class QMouseEvent;
class RideItem;
class MainWindow;
class QColor;
class QVBoxLayout;
class QTabWidget;
class GoogleMapControl;

// trick the maps api into ignoring gestures by
// pretending to be chrome. see: http://developer.qt.nokia.com/forums/viewthread/1643/P15
class myWebPage : public QWebPage
{
#if 0
    virtual QString userAgentForUrl(const QUrl&) const {
        return "Mozilla/5.0";
    }
#endif
};

class WebBridge : public QObject
{
    Q_OBJECT;

    private:
        MainWindow *mainWindow;
        GoogleMapControl *gm;

    public:
        WebBridge(MainWindow *mainWindow, GoogleMapControl *gm) : mainWindow(mainWindow), gm(gm) {}

    public slots:
        Q_INVOKABLE void call(int count);

        // drawing basic route, and interval polylines
        Q_INVOKABLE int intervalCount();
        Q_INVOKABLE QVariantList getLatLons(int i); // get array of latitudes for highlighted n

        // once map and basic route is loaded
        // this slot is called to draw additional
        // overlays e.g. shaded route, markers
        Q_INVOKABLE void drawOverlays();

        // display/toggle interval on map
        Q_INVOKABLE void toggleInterval(int);
        void intervalsChanged() { emit drawIntervals(); }

    signals:
        void drawIntervals();
};

class GoogleMapControl : public GcChartWindow
{
    Q_OBJECT
    G_OBJECT

    public:
        GoogleMapControl(MainWindow *);
        virtual ~GoogleMapControl() {}
        bool first;

    public slots:
        void rideSelected();
        void createMarkers();
        void drawShadedRoute();
        void zoomInterval(IntervalItem*);

    private:
        MainWindow *main;
        QVBoxLayout *layout;
        QWebView *view;
        MainWindow *parent;
        WebBridge *webBridge;
        GoogleMapControl();  // default ctor
        int range;
        int rideCP; // rider's CP
        QString currentPage;
        RideItem *current;

        QColor GetColor(int watts);
        void createHtml();

    private slots:
        void loadRide();
        void updateFrame();

};

#endif
