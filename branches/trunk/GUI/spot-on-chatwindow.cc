/*
** Copyright (c) 2011 - 10^10^10, Alexis Megas.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. The name of the author may not be used to endorse or promote products
**    derived from Spot-On without specific prior written permission.
**
** SPOT-ON IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
** IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
** IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
** NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
** SPOT-ON, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "Common/spot-on-misc.h"
#include "spot-on.h"
#include "spot-on-chatwindow.h"
#include "spot-on-defines.h"

#include <QDateTime>
#if QT_VERSION >= 0x050000
#include <QMediaPlayer>
#endif
#include <QMessageBox>
#include <QScrollBar>
#include <QSettings>
#include <QStandardItemModel>

spoton_chatwindow::spoton_chatwindow(const QIcon &icon,
				     const QString &id,
				     const QString &keyType,
				     const QString &participant,
				     const QString &publicKeyHash,
				     const QString &status,
				     QSslSocket *kernelSocket,
				     QWidget *parent):QMainWindow(parent)
{
  m_id = id;
  m_keyType = keyType.toLower();

  if(m_keyType.isEmpty())
    m_keyType = "chat";

  m_kernelSocket = kernelSocket;
  m_publicKeyHash = publicKeyHash;
  ui.setupUi(this);

#if defined(Q_OS_MAC) || defined(Q_OS_WIN32)
  QSettings settings;

  if(settings.value("gui/ontopChatDialogs", false).toBool())
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
#endif
#ifdef Q_OS_MAC
#if QT_VERSION < 0x050000
  setAttribute(Qt::WA_MacMetalStyle, true);
#endif
#if QT_VERSION >= 0x050000
  setWindowFlags(windowFlags() & ~Qt::WindowFullscreenButtonHint);
#endif
  statusBar()->setSizeGripEnabled(false);
#endif
  connect(ui.action_Close,
	  SIGNAL(triggered(void)),
	  this,
	  SLOT(close(void)));
  connect(ui.clearMessages,
	  SIGNAL(clicked(void)),
	  ui.messages,
	  SLOT(clear(void)));
  connect(ui.messages,
	  SIGNAL(anchorClicked(const QUrl &)),
	  this,
	  SIGNAL(anchorClicked(const QUrl &)));
  connect(ui.messages,
	  SIGNAL(anchorClicked(const QUrl &)),
	  this,
	  SLOT(slotLinkClicked(const QUrl &)));
  connect(ui.message,
	  SIGNAL(returnPressed(void)),
	  this,
	  SLOT(slotSendMessage(void)));
  connect(ui.sendMessage,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(slotSendMessage(void)));
  connect(ui.share,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(slotShareStarBeam(void)));
  connect(ui.starbeam,
	  SIGNAL(toggled(bool)),
	  ui.line,
	  SLOT(setVisible(bool)));
  connect(ui.starbeam,
	  SIGNAL(toggled(bool)),
	  ui.table,
	  SLOT(setVisible(bool)));

  if(participant.trimmed().isEmpty())
    {
      if(m_keyType == "chat")
	setWindowTitle("unknown");
      else
	setWindowTitle("unknown@unknown.org");
    }
  else
    setWindowTitle(participant.trimmed());

  ui.icon->setPixmap(icon.pixmap(QSize(16, 16)));

  if(!status.trimmed().isEmpty())
    ui.icon->setToolTip(status.trimmed());

  ui.line->setVisible(false);
#if QT_VERSION >= 0x050200
  ui.message->setPlaceholderText(tr("Please type a message..."));
#endif

  if(participant.trimmed().isEmpty())
    ui.name->setText("unknown");
  else
    ui.name->setText(participant.trimmed());

  ui.share->setEnabled(m_keyType != "poptastic");
  ui.starbeam->setEnabled(m_keyType != "poptastic");
  ui.table->resizeColumnToContents(0);
  ui.table->setModel(spoton::instance() ?
		     spoton::instance()->starbeamReceivedModel() : 0);
  ui.table->setVisible(false);

  QMenu *menu = new QMenu(this);

  menu->addAction(tr("&Reset the SMP machine's internal state to s0."),
		  this,
		  SLOT(slotInitializeSMP(void)));
  menu->addAction(tr("&Set an SMP Secret..."),
		  this,
		  SLOT(slotPrepareSMP(void)));
  menu->addAction(tr("&Verify the SMP secret."),
		  this,
		  SLOT(slotVerifySMPSecret(void)));
  menu->addSeparator();
  menu->addAction(tr("&Derive Gemini pair from SMP secret."),
		  this,
		  SLOT(slotDeriveGeminiPairViaSMP(void)));
  ui.smp->setMenu(menu);
  connect(ui.smp,
	  SIGNAL(clicked(void)),
	  ui.smp,
	  SLOT(showMenu(void)));
  slotSetIcons();
}

spoton_chatwindow::~spoton_chatwindow()
{
}

void spoton_chatwindow::slotSetIcons(void)
{
  QSettings settings;
  QString iconSet(settings.value("gui/iconSet", "nouve").toString().
		  toLower());

  if(!(iconSet == "everaldo" ||
       iconSet == "meego" ||
       iconSet == "nouve" ||
       iconSet == "nuvola"))
    iconSet = "nouve";

  ui.clearMessages->setIcon(QIcon(QString(":/%1/clear.png").arg(iconSet)));
  ui.sendMessage->setIcon(QIcon(QString(":/%1/ok.png").arg(iconSet)));
  ui.share->setIcon(QIcon(QString(":/%1/starbeam.png").arg(iconSet)));
}

QString spoton_chatwindow::id(void) const
{
  return m_id;
}

void spoton_chatwindow::closeEvent(QCloseEvent *event)
{
  QMainWindow::closeEvent(event);
}

void spoton_chatwindow::center(QWidget *parent)
{
  spoton::centerWidget(this, parent);
}

void spoton_chatwindow::slotSendMessage(void)
{
  sendMessage(0);
}

void spoton_chatwindow::sendMessage(bool *ok)
{
  QByteArray message;
  QByteArray name;
  QDateTime now(QDateTime::currentDateTime());
  QSettings settings;
  QString error("");
  QString msg("");

  if(m_kernelSocket->state() != QAbstractSocket::ConnectedState)
    {
      error = tr("The interface is not connected to the kernel.");
      goto done_label;
    }
  else if(!m_kernelSocket->isEncrypted())
    {
      error = tr("The connection to the kernel is not encrypted.");
      goto done_label;
    }
  else if(ui.message->toPlainText().isEmpty())
    {
      error = tr("Please provide a real message.");
      goto done_label;
    }

  if(m_keyType == "chat")
    name = spoton::instance() ? spoton::instance()->m_settings.
      value("gui/nodeName", "unknown").toByteArray() : "unknown";
  else
    name = spoton::instance() ? spoton::instance()->m_settings.
      value("gui/poptasticName", "unknown@unknown.org").toByteArray() :
      "unknown@unknown.org";

  msg.append
    (QString("[%1/%2/%3 %4:%5<font color=gray>:%6</font>] ").
     arg(now.toString("MM")).
     arg(now.toString("dd")).
     arg(now.toString("yyyy")).
     arg(now.toString("hh")).
     arg(now.toString("mm")).
     arg(now.toString("ss")));
  msg.append(tr("<b>me:</b> "));

  if(settings.value("gui/enableChatEmoticons", false).toBool())
    msg.append
      (spoton::mapIconToEmoticon(ui.message->toPlainText()));
  else
    msg.append(ui.message->toPlainText());

  ui.messages->append(msg);
  ui.messages->verticalScrollBar()->setValue
    (ui.messages->verticalScrollBar()->maximum());

  if(spoton::instance())
    {
      spoton::instance()->ui().messages->append(msg);
      spoton::instance()->ui().messages->verticalScrollBar()->setValue
	(spoton::instance()->ui().messages->verticalScrollBar()->maximum());
    }

  if(name.isEmpty())
    {
      if(m_keyType == "chat")
	name = "unknown";
      else
	name = "unknown@unknown.org";
    }

  if(spoton::instance())
    {
      if(!spoton::instance()->m_chatSequenceNumbers.contains(m_id))
	spoton::instance()->m_chatSequenceNumbers[m_id] = 0;

      spoton::instance()->m_chatSequenceNumbers[m_id] += 1;
    }

  if(m_keyType == "chat")
    message.append("message_");
  else
    message.append("poptasticmessage_");

  message.append(QString("%1_").arg(m_id));
  message.append(name.toBase64());
  message.append("_");
  message.append(ui.message->toPlainText().toUtf8().toBase64());
  message.append("_");
  message.append
    (QByteArray::number(spoton::instance() ?
			spoton::instance()->m_chatSequenceNumbers[m_id] :
			1).toBase64());
  message.append("_");
  message.append(QDateTime::currentDateTime().toUTC().
		 toString("MMddyyyyhhmmss").toLatin1().toBase64());
  message.append("\n");

  if(spoton::instance())
    spoton::instance()->addMessageToReplayQueue(msg, message, m_publicKeyHash);

  if(m_kernelSocket->write(message.constData(), message.length()) !=
     message.length())
    {
      error = tr("An error occurred while writing to the "
		 "kernel socket.");
      spoton_misc::logError
	(QString("spoton_chatwindow::slotSendMessage(): write() failure for "
		 "%1:%2.").
	 arg(m_kernelSocket->peerAddress().toString()).
	 arg(m_kernelSocket->peerPort()));
      goto done_label;
    }
  else
    emit messageSent();

  ui.message->clear();

 done_label:

#if QT_VERSION >= 0x050000
  if(error.isEmpty())
    {
      QMediaPlayer *player = findChild<QMediaPlayer *> ();

      if(player)
	player->deleteLater();

      if(spoton::instance() ? spoton::instance()->m_settings.
	 value("gui/play_sounds", false).toBool() : false)
	{
	  QFileInfo fileInfo;
	  QString str
	    (QDir::cleanPath(QCoreApplication::applicationDirPath() +
			     QDir::separator() + "Sounds" + QDir::separator() +
			     "send.wav"));

	  fileInfo.setFile(str);

	  if(!(!fileInfo.isReadable() || fileInfo.size() < 8192))
	    {
	      player = new (std::nothrow) QMediaPlayer
		(this, QMediaPlayer::LowLatency);

	      if(player)
		{
		  player->setMedia(QUrl::fromLocalFile(str));
		  player->setVolume(100);
		  player->play();
		}
	      else
		QApplication::beep();
	    }
	}
    }
#else
  if(error.isEmpty())
    QApplication::beep();
#endif

  if(!error.isEmpty())
    {
      if(ok)
	*ok = false;
      else
	QMessageBox::critical(this, tr("%1: Error").
			      arg(SPOTON_APPLICATION_NAME), error);
    }
}

void spoton_chatwindow::append(const QString &text)
{
  ui.messages->append(text);
  ui.messages->verticalScrollBar()->setValue
    (ui.messages->verticalScrollBar()->maximum());
}

void spoton_chatwindow::slotSetStatus(const QIcon &icon,
				      const QString &name,
				      const QString &id,
				      const QString &status)
{
  if(id == m_id)
    {
      if(!icon.isNull())
	{
	  ui.icon->setPixmap(icon.pixmap(QSize(16, 16)));

	  if(!status.trimmed().isEmpty())
	    ui.icon->setToolTip(status.trimmed());
	}

      if(!name.trimmed().isEmpty())
	{
	  setWindowTitle(name.trimmed());
	  ui.name->setText(name.trimmed());
	}
      else
	{
	  if(m_keyType == "chat")
	    {
	      setWindowTitle("unknown");
	      ui.name->setText("unknown");
	    }
	  else
	    {
	      setWindowTitle("unknown@unknown.org");
	      ui.name->setText("unknown@unknown.org");
	    }
	}
    }
}

void spoton_chatwindow::setName(const QString &name)
{
  if(!name.trimmed().isEmpty())
    {
      setWindowTitle(name.trimmed());
      ui.name->setText(name.trimmed());
    }
  else
    {
      if(m_keyType == "chat")
	{
	  setWindowTitle("unknown");
	  ui.name->setText("unknown");
	}
      else
	{
	  setWindowTitle("unknown@unknown.org");
	  ui.name->setText("unknown@unknown.org");
	}
    }
}

void spoton_chatwindow::keyPressEvent(QKeyEvent *event)
{
  if(event)
    {
      if(event->key() == Qt::Key_Escape)
	close();
    }

  QMainWindow::keyPressEvent(event);
}

#ifdef Q_OS_MAC
#if QT_VERSION >= 0x050000 && QT_VERSION < 0x050300
bool spoton_chatwindow::event(QEvent *event)
{
  if(event)
    if(event->type() == QEvent::WindowStateChange)
      if(windowState() == Qt::WindowNoState)
	{
	  /*
	  ** Minimizing the window on OS 10.6.8 and Qt 5.x will cause
	  ** the window to become stale once it has resurfaced.
	  */

	  hide();
	  show();
	  update();
	}

  return QMainWindow::event(event);
}
#endif
#endif

void spoton_chatwindow::setSMPVerified(const bool state)
{
  QDateTime now(QDateTime::currentDateTime());

  if(!state)
    {
      ui.smp->setIcon(QIcon(":/generic/smp-unlocked.png"));
      ui.smp->setToolTip
	(tr("The Socialist Millionaire Protocol "
	    "failed on %1.").
	 arg(now.toString(Qt::ISODate)));
    }
  else
    {
      ui.smp->setIcon(QIcon(":/generic/smp-locked.png"));
      ui.smp->setToolTip
	(tr("The Socialist Millionaire Protocol succeeded on %1.").
	 arg(now.toString(Qt::ISODate)));
    }
}

void spoton_chatwindow::slotDeriveGeminiPairViaSMP(void)
{
  emit deriveGeminiPairViaSMP(m_publicKeyHash, m_id);
}

void spoton_chatwindow::slotInitializeSMP(void)
{
  emit initializeSMP(m_publicKeyHash);
}

void spoton_chatwindow::slotPrepareSMP(void)
{
  emit prepareSMP(m_publicKeyHash);
}

void spoton_chatwindow::slotVerifySMPSecret(void)
{
  emit verifySMPSecret(m_publicKeyHash, m_keyType, m_id);
}

void spoton_chatwindow::slotShareStarBeam(void)
{
  QString error("");
  spoton_crypt *crypt = spoton::instance() ?
    spoton::instance()->crypts().value("chat", 0) : 0;

  if(!crypt)
    {
      error = tr("Invalid spoton_crypt object. This is a fatal flaw.");
      showError(error);
      return;
    }

  /*
  ** Some of this logic is redundant. Please see sendMessage().
  */

  if(m_kernelSocket->state() != QAbstractSocket::ConnectedState)
    {
      error = tr("The interface is not connected to the kernel.");
      showError(error);
      return;
    }
  else if(!m_kernelSocket->isEncrypted())
    {
      error = tr("The connection to the kernel is not encrypted.");
      showError(error);
      return;
    }

  /*
  ** Select a file.
  */

  QFileDialog dialog(this);

  dialog.setWindowTitle(tr("%1: Select StarBeam Transmit File").
			arg(SPOTON_APPLICATION_NAME));
  dialog.setFileMode(QFileDialog::ExistingFile);
  dialog.setDirectory(QDir::homePath());
  dialog.setLabelText(QFileDialog::Accept, tr("Select"));
  dialog.setAcceptMode(QFileDialog::AcceptOpen);
#ifdef Q_OS_MAC
#if QT_VERSION < 0x050000
  dialog.setAttribute(Qt::WA_MacMetalStyle, false);
#endif
#endif

  if(dialog.exec() != QDialog::Accepted)
    return;

  QFileInfo fileInfo(dialog.selectedFiles().value(0));

  if(!fileInfo.exists() || !fileInfo.isReadable())
    {
      error = tr("The selected file is not readable.");
      showError(error);
      return;
    }

  spoton::prepareDatabasesFromUI();

  /*
  ** Create a StarBeam magnet.
  */

  QByteArray eKey(spoton_crypt::strongRandomBytes(spoton_crypt::
						  cipherKeyLength("aes256")).
		  toBase64());
  QByteArray mKey(spoton_crypt::
		  strongRandomBytes(spoton_crypt::
				    XYZ_DIGEST_OUTPUT_SIZE_IN_BYTES).
		  toBase64());
  QByteArray magnet;
  bool ok = true;

  magnet.append("magnet:?");
  magnet.append("ct=aes256&");
  magnet.append("ek=");
  magnet.append(eKey);
  magnet.append("&");
  magnet.append("ht=sha512&");
  magnet.append("mk=");
  magnet.append(mKey);
  magnet.append("&");
  magnet.append("xt=urn:starbeam");
  ui.message->setText(magnet);
  sendMessage(&ok);

  if(!ok)
    return;

  QString connectionName("");

  /*
  ** Create a StarBeam database entry.
  */

  {
    QSqlDatabase db = spoton_misc::database(connectionName);

    db.setDatabaseName(spoton_misc::homePath() + QDir::separator() +
		       "starbeam.db");

    if(db.open())
      {
	QByteArray encryptedMosaic;
	QByteArray mosaic
	  (spoton_crypt::strongRandomBytes(spoton_common::MOSAIC_SIZE).
	   toBase64());
	QSqlQuery query(db);

	query.prepare("INSERT INTO transmitted "
		      "(file, hash, missing_links, mosaic, nova, "
		      "position, pulse_size, read_interval, "
		      "status_control, total_size) "
		      "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
	query.bindValue
	  (0, crypt->
	   encryptedThenHashed(fileInfo.absoluteFilePath().toUtf8(),
			       &ok).toBase64());

	if(ok)
	  query.bindValue
	    (1, crypt->
	     encryptedThenHashed
	     (spoton_crypt::
	      sha1FileHash(fileInfo.absoluteFilePath()).toHex(),
	      &ok).toBase64());

	if(ok)
	  query.bindValue
	    (2, crypt->
	     encryptedThenHashed(QByteArray(), &ok).toBase64());

	if(ok)
	  {
	    encryptedMosaic = crypt->encryptedThenHashed(mosaic, &ok);

	    if(ok)
	      query.bindValue(3, encryptedMosaic.toBase64());
	  }

	if(ok)
	  query.bindValue
	    (4, crypt->encryptedThenHashed(QByteArray(), &ok).toBase64());

	if(ok)
	  query.bindValue
	    (5, crypt->encryptedThenHashed("0", &ok).toBase64());

	if(ok)
	  query.bindValue
	    (6, crypt->
	     encryptedThenHashed(QByteArray::number(30000),
				 &ok).toBase64());

	query.bindValue(7, 2.500);
	query.bindValue(8, "transmitting");

	if(ok)
	  query.bindValue
	    (9, crypt->
	     encryptedThenHashed(QByteArray::number(fileInfo.size()),
				 &ok).toBase64());

	if(ok)
	  query.exec();

	query.prepare("INSERT INTO transmitted_magnets "
		      "(magnet, magnet_hash, transmitted_oid) "
		      "VALUES (?, ?, (SELECT OID FROM transmitted WHERE "
		      "mosaic = ?))");

	if(ok)
	  query.bindValue
	    (0, crypt->
	     encryptedThenHashed(magnet, &ok).toBase64());

	if(ok)
	  query.bindValue
	    (1, crypt->keyedHash(magnet, &ok).toBase64());

	if(ok)
	  query.bindValue(2, encryptedMosaic.toBase64());

	if(ok)
	  ok = query.exec();
      }
    else
      ok = false;

    db.close();
  }

  QSqlDatabase::removeDatabase(connectionName);

  if(!ok)
    {
      error = tr("An error occurred while attempting to "
		 "save the StarBeam data. Please enable "
		 "logging via the Log Viewer and try again.");
      showError(error);
    }
}

void spoton_chatwindow::showError(const QString &error)
{
  if(error.trimmed().isEmpty())
    return;

  QMessageBox::critical(this, tr("%1: Error").
			arg(SPOTON_APPLICATION_NAME), error.trimmed());
}

void spoton_chatwindow::showNormal(void)
{
#if defined(Q_OS_MAC) || defined(Q_OS_WIN32)
  QSettings settings;

  if(settings.value("gui/ontopChatDialogs", false).toBool())
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
  else
    setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
#endif
  QMainWindow::showNormal();
}

void spoton_chatwindow::slotLinkClicked(const QUrl &url)
{
  QString scheme(url.scheme().toLower().trimmed());

  if(!(scheme == "ftp" || scheme == "http" || scheme == "https"))
    return;

  QSettings settings;

  if(!settings.value("gui/openChatUrl", false).toBool())
    return;

  QMessageBox mb(this);
  QString str(spoton_misc::urlToEncoded(url).constData());

  if(str.length() > 64)
    str = str.mid(0, 24) + "..." + str.right(24);

#ifdef Q_OS_MAC
#if QT_VERSION < 0x050000
  mb.setAttribute(Qt::WA_MacMetalStyle, true);
#endif
#endif
  mb.setIcon(QMessageBox::Question);
  mb.setWindowTitle(tr("%1: Confirmation").
		    arg(SPOTON_APPLICATION_NAME));
  mb.setWindowModality(Qt::WindowModal);
  mb.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
  mb.setText(tr("Are you sure that you wish to access %1?").arg(str));

  if(mb.exec() != QMessageBox::Yes)
    return;

  QDesktopServices::openUrl(url);
}
