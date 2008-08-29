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
 * NVideoPlayer subroutines.
 *
 * REVISION:
 * 
 * 1) Initial creation. ----------------------------------- 2008-08-28 WX
 *
 */

//#define QT_NO_DEBUG_OUTPUT
#include <QDebug>
#include <QTimer>
#include <QPixmapCache>
#include "nvideoplayer.h"

#define IMAGE_BACKGROUND_VIDEOBAR (":/backgrounds/bg-playbackframe.png")
#define IMAGE_BACKGROUND_PLAYSTATE (":/backgrounds/bg-playstateframe.png")
#define ICON_PLAYSTATE_PLAY (":/video/play.png")
#define ICON_PLAYSTATE_PAUSE (":/video/pause.png")
#define ICON_PLAYSTATE_FF (":/video/ff.png")
#define ICON_PLAYSTATE_REW (":/video/rw.png")

#define DEF_PLAYTIMER_INTERVAL (500) //ms
#define DEF_VIDEOBAR_INTERVAL (10*1000) //10s


NVideoPlayer::NVideoPlayer(QWidget *parent)
: QWidget(parent)
{
    setupUi(this);

    /* test !!! */
    timeProgressBar->setMinimum(0);
    timeProgressBar->setMaximum(100);

    /* Initial controls */
    stateRateLabel->setVisible(false);

    /* Initial Xim menu and actions */
    cleanBookMarkAction = new NAction(tr("Clean book marks"), this);
    setBookMarkAction = new NAction(tr("Bookmark"), this);
    jumpToStartAction = new NAction(tr("Jump to Start"), this);
    jumpToLocationAction = new NAction(tr("Jump to location"), this);
    chooseSubtitleAction = new NAction(tr("Choose Subtitle File"), this);
    setProportionAction = new NAction(this);
    settingsAction = new NAction(tr("Play Mode Settings"), this);
    showHideDetailAction = new NAction(this);
    for (int i=0 ;i<9; i++)
        goToMarkActions += new NAction(tr("Goto Bookmark %1").arg(QString::number(i + 1)),this);

    /* Initial stamp timer */
    playTimer = new QTimer(this);
    playTimer->setInterval(DEF_PLAYTIMER_INTERVAL);
    videoBarInterval = DEF_VIDEOBAR_INTERVAL;

    /* connect signals and slots */
    connect(playTimer, SIGNAL(timeout()), this, SLOT(OnPlayTimerOut()));
    connect(cleanBookMarkAction, SIGNAL(triggered()), this, SLOT(OnCleanBookMark()));
    connect(setBookMarkAction, SIGNAL(triggered()), this, SLOT(OnSetBookMark()));
    connect(jumpToStartAction, SIGNAL(triggered()), this, SLOT(OnJumpToStart()));
    connect(jumpToLocationAction, SIGNAL(triggered()), this, SLOT(OnJumpToLocation()));
    connect(chooseSubtitleAction, SIGNAL(triggered()), this, SLOT(OnChooseSubtitle()));
    connect(setProportionAction, SIGNAL(triggered()), this, SLOT(OnSetProportion()));
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(OnSettings()));
    connect(showHideDetailAction, SIGNAL(triggered()), this, SLOT(OnShowHideDetail()));
    connect(showHideDetailAction, SIGNAL(triggered()), this, SLOT(OnShowHideDetail()));
    Q_FOREACH(NAction *item, goToMarkActions)
        connect(item, SIGNAL(triggered()), this, SLOT(OnGoToBookMark()));
}

NVideoPlayer::~NVideoPlayer()
{
    if (!xiMenu.isNull())
        delete xiMenu;
    if (!settingsForm.isNull())
        delete settingsForm;

    delete playTimer;
    delete cleanBookMarkAction;
    qDeleteAll(goToMarkActions);
    delete setBookMarkAction;
    delete jumpToStartAction;
    delete jumpToLocationAction;
    delete chooseSubtitleAction;
    delete setProportionAction;
    delete settingsAction;
    delete showHideDetailAction;
}

void NVideoPlayer::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    QPixmap image;
    /* load the background frame image for the video bar */
    image.load(IMAGE_BACKGROUND_VIDEOBAR);
    if (!image.isNull())
    {
        QPalette pal = playBarFrame->palette();
        QBrush b = QBrush(image.scaled(playBarFrame->size()));
        pal.setBrush(backgroundRole(), b);
        playBarFrame->setPalette(pal);
        playBarFrame->setAutoFillBackground(true);
    }
    /* load the background frame image for the video bar */
    image.load(IMAGE_BACKGROUND_PLAYSTATE);
    if (!image.isNull())
    {
        QPalette pal = playStateFrame->palette();
        QBrush b = QBrush(image.scaled(playStateFrame->size()));
        pal.setBrush(backgroundRole(), b);
        playStateFrame->setPalette(pal);
        playStateFrame->setAutoFillBackground(true);
    }
}

void NVideoPlayer::keyPressEvent(QKeyEvent *event)
{
    /* reset the video bar interval if any key(except the Menu key) pressed */
    bool barShow = false;
    if (event->key() != Qt::Key_Menu)
    {
        barShow = !videoBarFrame->isVisible();
        videoBarInterval = DEF_VIDEOBAR_INTERVAL;
        videoBarFrame->setVisible(true);
    }

    switch (event->key())
    {
    case Qt::Key_MediaPlay:
    case Qt::Key_Enter:
        OnPauseUnPause();
        break;
    case Qt::Key_MediaStop:
    case Qt::Key_Escape:
        OnStop();
        break;
    case Qt::Key_MediaPrevious:
        OnPrevious();
        break;
    case Qt::Key_MediaNext:
        OnNext();
        break;
    case Qt::Key_Forward:
    case Qt::Key_Right:
        OnFastFoward();
        break;
    case Qt::Key_Back:
    case Qt::Key_Left:
        OnRewind();
        break;
    case Qt::Key_Menu:
    case Qt::Key_F11: // F11: test key on Qt-X11
        OnMenu();
        break;
    case Qt::Key_F12: //DISP Key
        if (!barShow)
            videoBarFrame->setVisible(!videoBarFrame->isVisible());
        break;
    case Qt::Key_Up:
        OnSetBookMark();
        break;
    case Qt::Key_Down:
        OnSetClipMark();
        break;
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
        seekToBookMark(event->key() - Qt::Key_1);
        break;
    case Qt::Key_Asterisk:
        //seek forward by step
        break;
    case Qt::Key_Clear:
        //seek backward by step
        break;
    default:
        break;
    }
}

bool NVideoPlayer::play()
{
    playTimer->start();

    return true;
}

void NVideoPlayer::stop()
{
}

void NVideoPlayer::OnPlayTimerOut()
{
    /* check video bar interval */
    if (videoBarFrame->isVisible()
        && (xiMenu.isNull() || !xiMenu->isVisible())
        && (settingsForm.isNull() || !settingsForm->isVisible()))
    {
        videoBarInterval -= playTimer->interval();
        if (videoBarInterval <=0)
        {
            videoBarInterval = DEF_VIDEOBAR_INTERVAL;
            videoBarFrame->setVisible(false);
        }
    }

    /* test the time progress bar */
    static int i = 0;
    if (i > 100)
        i = 0;
    timeProgressBar->setValue(i ++);
}

void NVideoPlayer::OnPauseUnPause()
{
    setPlayStateIcon(0);
}

void NVideoPlayer::OnStop()
{
    close();
}

void NVideoPlayer::OnNext()
{
}

void NVideoPlayer::OnPrevious()
{
}

void NVideoPlayer::OnFastFoward()
{
    setPlayStateIcon(2);
}

void NVideoPlayer::OnRewind()
{
    setPlayStateIcon(3);
}


void NVideoPlayer::OnMenu()
{
    if (xiMenu.isNull())
    {
        xiMenu = new NXim(this);
        xiMenu->setFont(font());
    }

    setProportionAction->setText(true ? tr("Display in 16:9 proportions") : tr("Display in 4:3 proportions"));
    showHideDetailAction->setText(videoBarFrame->isVisible() ? tr("Hide Detail") : tr("Show Detail"));
    xiMenu->clear();

    if (true)
        xiMenu->addAction(jumpToStartAction);
    if (true)
        xiMenu->addAction(setBookMarkAction);
    if (true)
        Q_FOREACH(NAction *item, goToMarkActions)
            xiMenu->addAction(item);
    if (true)
        xiMenu->addAction(cleanBookMarkAction);
    if (true)
        xiMenu->addAction(setProportionAction);
    if (true)
        xiMenu->addAction(jumpToLocationAction);
    if (true)
        xiMenu->addAction(settingsAction);
    if (true)
        xiMenu->addAction(chooseSubtitleAction);
    if (true)
        xiMenu->addAction(showHideDetailAction);

    xiMenu->show();
}

void NVideoPlayer::OnCleanBookMark()
{
}

void NVideoPlayer::OnGoToBookMark()
{
}

void NVideoPlayer::OnSetBookMark()
{
}

void NVideoPlayer::OnSetClipMark()
{
}

void NVideoPlayer::OnJumpToStart()
{
}

void NVideoPlayer::OnJumpToLocation()
{
}

void NVideoPlayer::OnChooseSubtitle()
{
}

void NVideoPlayer::OnSetProportion()
{
}

void NVideoPlayer::OnSettings()
{
    if (settingsForm.isNull())
    {
        settingsForm = new NVideoPlaybackSettings(this);
        settingsForm->setAttribute(Qt::WA_DeleteOnClose, true);
        connect(settingsForm, SIGNAL(completed()), this, SLOT(OnSettingsCompleted()));
        connect(settingsForm, SIGNAL(cancelled()), this, SLOT(OnSettingsCancelled()));
    }

    settingsForm->showMaximized();
}

void NVideoPlayer::OnShowHideDetail()
{
    videoBarFrame->setVisible(!videoBarFrame->isVisible());
}

void NVideoPlayer::OnSettingsCompleted()
{
}

void NVideoPlayer::OnSettingsCancelled()
{
}

void NVideoPlayer::seekToBookMark(int index)
{

}

void NVideoPlayer::setPlayStateIcon(int stat)
{
    QString iconName;
    switch (stat)
    {
    case 0: //PLAY
        iconName = ICON_PLAYSTATE_PLAY;
        break;
    case 1: //PAUSE
        iconName = ICON_PLAYSTATE_PAUSE;
        break;
    case 2: //FF
        iconName = ICON_PLAYSTATE_FF;
        break;
    case 3: //REW
        iconName = ICON_PLAYSTATE_REW;
        break;
    default:
        break;
    }

    if (iconName.isEmpty())
    {
        stateIconLabel->clear();
        return;
    }

    QPixmap pixmap;
    if (!QPixmapCache::find(iconName, pixmap))
    {
        pixmap.load(iconName);
        QPixmapCache::insert(iconName, pixmap);
    }

    stateIconLabel->setPixmap(pixmap.scaled(stateIconLabel->size()));
}

