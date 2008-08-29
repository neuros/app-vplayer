#ifndef _NVIDEOPLAYER_H_
#define _NVIDEOPLAYER_H_
/*
 *  Copyright(C) 2007 Neuros Technology International LLC. 
 *               <www.neurostechnology.com>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 2 of the License.
 *  
 *
 *  This program is distributed in the hope that, in addition to its 
 *  original purpose to support Neuros hardware, it will be useful 
 *  otherwise, but WITHOUT ANY WARRANTY; without even the implied 
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 ****************************************************************************
 *
 * NVideoPlayer header.
 *
 * REVISION:
 * 
 * 1) Initial creation. ----------------------------------- 2008-08-28 WX
 *
 */

#include <QPointer>
#include "ui_nvideoplayer.h"
#include "nvideoplaybacksettings.h"
#include "nxim.h"
#include "naction.h"

class NVideoPlayer: public QWidget, private Ui::NVideoPlayer
{
    Q_OBJECT

public:
    NVideoPlayer(QWidget *parent = NULL);
    ~NVideoPlayer();

Q_SIGNALS:

public Q_SLOTS:
    bool play();
    void stop();

protected:
    void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *event);

private Q_SLOTS:
    /* time stamp slot */
    void OnPlayTimerOut();
    /* key pressed responsed slots */
    void OnPauseUnPause();
    void OnStop();
    void OnNext();
    void OnPrevious();
    void OnFastFoward();
    void OnRewind();
    void OnMenu();
    /* menu actions slots */
    void OnCleanBookMark();
    void OnGoToBookMark();
    void OnSetBookMark();
    void OnSetClipMark();
    void OnJumpToStart();
    void OnJumpToLocation();
    void OnChooseSubtitle();
    void OnSetProportion();
    void OnSettings();
    void OnShowHideDetail();
    /* settings form slots */
    void OnSettingsCompleted();
    void OnSettingsCancelled();

private:
    /* help api */
    void seekToBookMark(int index);
    void setPlayStateIcon(int stat);

private:
    /* stamp timer */
    QTimer *playTimer;
    /* video bar display interval */
    int videoBarInterval;
    /* menu actions */
    NAction *cleanBookMarkAction;
    QList<NAction *> goToMarkActions;
    NAction *setBookMarkAction;
    NAction *jumpToStartAction;
    NAction *jumpToLocationAction;
    NAction *chooseSubtitleAction;
    NAction *setProportionAction;
    NAction *settingsAction;
    NAction *showHideDetailAction;
    /* sub windows */
    QPointer<NXim> xiMenu;
    QPointer<NVideoPlaybackSettings> settingsForm;
};

#endif // _NVIDEOPLAYER_H_
