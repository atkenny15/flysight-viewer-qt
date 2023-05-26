/***************************************************************************
**                                                                        **
**  FlySight Viewer                                                       **
**  Copyright 2018 Michael Cooper                                         **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see <http://www.gnu.org/licenses/>. **
**                                                                        **
****************************************************************************
**  Contact: Michael Cooper                                               **
**  Website: http://flysight.ca/                                          **
****************************************************************************/

#include "datapoint.h"

DataPoint DataPoint::interpolate(const DataPoint& p1, const DataPoint& p2, double a) {
    DataPoint ret;

    const qint64 ms1 = p1.dateTime.toMSecsSinceEpoch();
    const qint64 ms2 = p2.dateTime.toMSecsSinceEpoch();
    ret.dateTime = QDateTime::fromMSecsSinceEpoch(ms1 + (qint64)(a * (ms2 - ms1)));

    ret.hasGeodetic = p1.hasGeodetic && p2.hasGeodetic;

    ret.lat = p1.lat + a * (p2.lat - p1.lat);
    ret.lon = p1.lon + a * (p2.lon - p1.lon);
    ret.hMSL = p1.hMSL + a * (p2.hMSL - p1.hMSL);

    ret.velN = p1.velN + a * (p2.velN - p1.velN);
    ret.velE = p1.velE + a * (p2.velE - p1.velE);
    ret.velD = p1.velD + a * (p2.velD - p1.velD);

    ret.hAcc = p1.hAcc + a * (p2.hAcc - p1.hAcc);
    ret.vAcc = p1.vAcc + a * (p2.vAcc - p1.vAcc);
    ret.sAcc = p1.sAcc + a * (p2.sAcc - p1.sAcc);

    if (a < 0.5)
        ret.numSV = p1.numSV;
    else
        ret.numSV = p2.numSV;

    ret.t = p1.t + a * (p2.t - p1.t);
    ret.x = p1.x + a * (p2.x - p1.x);
    ret.y = p1.y + a * (p2.y - p1.y);
    ret.z = p1.z + a * (p2.z - p1.z);

    ret.dist2D = p1.dist2D + a * (p2.dist2D - p1.dist2D);
    ret.dist3D = p1.dist3D + a * (p2.dist3D - p1.dist3D);

    ret.curv = p1.curv + a * (p2.curv - p1.curv);
    ret.accel = p1.accel + a * (p2.accel - p1.accel);

    ret.ax = p1.ax + a * (p2.ax - p1.ax);
    ret.ay = p1.ay + a * (p2.ay - p1.ay);
    ret.az = p1.az + a * (p2.az - p1.az);
    ret.amag = p1.amag + a * (p2.amag - p1.amag);

    ret.lift = p1.lift + a * (p2.lift - p1.lift);
    ret.drag = p1.drag + a * (p2.drag - p1.drag);

    ret.heading = p1.heading + a * (p2.heading - p1.heading);
    ret.cAcc = p1.cAcc + a * (p2.cAcc - p1.cAcc);

    ret.vx = p1.vx + a * (p2.vx - p1.vx);
    ret.vy = p1.vy + a * (p2.vy - p1.vy);

    ret.theta = p1.theta + a * (p2.theta - p1.theta);
    ret.omega = p1.omega + a * (p2.omega - p1.omega);

    return ret;
}
