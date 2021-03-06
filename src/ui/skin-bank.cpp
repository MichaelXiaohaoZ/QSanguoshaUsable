#include "skin-bank.h"
#include "json.h"
#include "protocol.h"
#include "ui-utils.h"
#include "engine.h"
#include "settings.h"
#include "clientstruct.h"
#include "settings.h"

#include <QFile>
#include <QGraphicsPixmapItem>
#include <QTextItem>
#include <QStyleOptionGraphicsItem>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QLabel>
#include <QGraphicsProxyWidget>
#include <QFile>
#include <QPixmapCache>

using namespace JsonUtils;

const char *IQSanComponentSkin::S_SKIN_KEY_DEFAULT = "default";
const char *IQSanComponentSkin::S_SKIN_KEY_DEFAULT_SECOND = "default2";
const char *QSanRoomSkin::S_SKIN_KEY_PHOTO = "photo";
const char *QSanRoomSkin::S_SKIN_KEY_ROOM = "room";
const char *QSanRoomSkin::S_SKIN_KEY_TABLE_BG = "tableBg";
const char *QSanRoomSkin::S_SKIN_KEY_COMMON = "common";
const char *QSanRoomSkin::S_SKIN_KEY_DASHBOARD = "dashboard";

// buttons
const char *QSanRoomSkin::S_SKIN_KEY_BUTTON = "button-%1";
const char *QSanRoomSkin::S_SKIN_KEY_DASHBOARD_BUTTON_BUTTOM = "dashboardButtonSetButtom";
const char *QSanRoomSkin::S_SKIN_KEY_DASHBOARD_BUTTON_BUTTOM_OLD = "dashboardButtonSetButtomOld";
const char *QSanRoomSkin::S_SKIN_KEY_DASHBOARD_BUTTON_SET_BG[7] =
    {"dashboardButtonSetBg", "dashboardButtonSetBg1", "dashboardButtonSetBg2", "dashboardButtonSetBg3",
                              "dashboardButtonSetBg4", "dashboardButtonSetBg5", "dashboardButtonSetBg6"};
const char *QSanRoomSkin::S_SKIN_KEY_DASHBOARD_BUTTON_SET_BG_OLD[7] =
    {"dashboardButtonSetBgOld", "dashboardButtonSetBgOld1", "dashboardButtonSetBgOld2", "dashboardButtonSetBgOld3",
                              "dashboardButtonSetBgOld4", "dashboardButtonSetBgOld5", "dashboardButtonSetBgOld6"};
const char *QSanRoomSkin::S_SKIN_KEY_BUTTON_SKILL = "skill";

// player container
const char *QSanRoomSkin::S_SKIN_KEY_EQUIP_ICON = "%1Equip-%2";
const char *QSanRoomSkin::S_SKIN_KEY_HORSE_ICON = "%1Horse-%2";
const char *QSanRoomSkin::S_SKIN_KEY_MAINFRAME[7] =
    {"%1MainFrame", "%1MainFrame1", "%1MainFrame2", "%1MainFrame3", "%1MainFrame4", "%1MainFrame5", "%1MainFrame6"};
const char *QSanRoomSkin::S_SKIN_KEY_LEFTFRAME[7] =
    { "%1LeftFrame", "%1LeftFrame1", "%1LeftFrame2", "%1LeftFrame3", "%1LeftFrame4", "%1LeftFrame5", "%1LeftFrame6"};
const char *QSanRoomSkin::S_SKIN_KEY_RIGHTFRAME[7] =
    {"%1RightFrame", "%1RightFrame1", "%1RightFrame2", "%1RightFrame3", "%1RightFrame4", "%1RightFrame5", "%1RightFrame6"};
const char *QSanRoomSkin::S_SKIN_KEY_MIDDLEFRAME[7] =
    {"%1MiddleFrame", "%1MiddleFrame1", "%1MiddleFrame2", "%1MiddleFrame3", "%1MiddleFrame4", "%1MiddleFrame5", "%1MiddleFrame6"};
const char *QSanRoomSkin::S_SKIN_KEY_FLOWERFRAME = "%1FlowerFrame";
const char *QSanRoomSkin::S_SKIN_KEY_HANDCARDNUM = "%1HandCardNum-%2";
const char *QSanRoomSkin::S_SKIN_KEY_FACETURNEDMASK = "%1FaceTurnedMask";
const char *QSanRoomSkin::S_SKIN_KEY_BLANK_GENERAL = "%1BlankGeneral";
const char *QSanRoomSkin::S_SKIN_KEY_CHAIN = "%1Chain";
const char *QSanRoomSkin::S_SKIN_KEY_PHASE = "%1Phase%2";
const char *QSanRoomSkin::S_SKIN_KEY_SELECTED_FRAME = "%1FrameWhenSelected";
const char *QSanRoomSkin::S_SKIN_KEY_FOCUS_FRAME = "%1FocusFrame%2";
const char *QSanRoomSkin::S_SKIN_KEY_KINGDOM_ICON = "kingdomIcon-%1";
const char *QSanRoomSkin::S_SKIN_KEY_KINGDOM_COLOR_MASK = "kingdomColorMask-%1";
const char *QSanRoomSkin::S_SKIN_KEY_DASHBOARD_KINGDOM_COLOR_MASK = "dashboardKingdomColorMask-%1";
const char *QSanRoomSkin::S_SKIN_KEY_VOTES_NUMBER = "votesNum-%1";
const char *QSanRoomSkin::S_SKIN_KEY_SEAT_NUMBER = "%1SeatNum-%2";
const char *QSanRoomSkin::S_SKIN_KEY_SAVE_ME_ICON = "saveMe";
const char *QSanRoomSkin::S_SKIN_KEY_ACTIONED_ICON = "playerActioned";
const char *QSanRoomSkin::S_SKIN_KEY_HAND_CARD_BACK[7] =
    {"handCardBack", "handCardBack1", "handCardBack2", "handCardBack3", "handCardBack4", "handCardBack5", "handCardBack6"};
const char *QSanRoomSkin::S_SKIN_KEY_HAND_CARD_SUIT = "handCardSuit-%1";
const char *QSanRoomSkin::S_SKIN_KEY_JUDGE_CARD_ICON = "judgeCardIcon-%1";
const char *QSanRoomSkin::S_SKIN_KEY_HAND_CARD_FRAME = "handCardFrame-%1";
const char *QSanRoomSkin::S_SKIN_KEY_HAND_CARD_MAIN_PHOTO = "handCardMainPhoto-%1";
const char *QSanRoomSkin::S_SKIN_KEY_HAND_CARD_NUMBER_BLACK = "handCardNumber-black-%1";
const char *QSanRoomSkin::S_SKIN_KEY_HAND_CARD_NUMBER_RED = "handCardNumber-red-%1";
const char *QSanRoomSkin::S_SKIN_KEY_GENERAL_CARD = "generalCard-%2-%1";
const char *QSanRoomSkin::S_SKIN_KEY_PLAYER_AUDIO_EFFECT = "playerAudioEffect-%1-%2";
const char *QSanRoomSkin::S_SKIN_KEY_SYSTEM_AUDIO_EFFECT = "systemAudioEffect-%1";
const char *QSanRoomSkin::S_SKIN_KEY_PLAYER_GENERAL_ICON = "playerGeneralIcon-%3-%2-%1";
const char *QSanRoomSkin::S_SKIN_KEY_MAGATAMAS_BG = "magatamasBg%1";
const char *QSanRoomSkin::S_SKIN_KEY_MAGATAMAS = "%1Magatamas%2";
const char *QSanRoomSkin::S_SKIN_KEY_MAGATAMAS_NUMBER = "MagatamasNumber-%1-%2";
const char *QSanRoomSkin::S_SKIN_KEY_PROGRESS_BAR_IMAGE = "progressBar";
const char *QSanRoomSkin::S_SKIN_KEY_GENERAL_CIRCLE_IMAGE = "generalCircleImage-%1";
const char *QSanRoomSkin::S_SKIN_KEY_GENERAL_CIRCLE_MASK = "generalCircleMask-%1";
const char *QSanRoomSkin::S_SKIN_KEY_SKILL_NAME_BG = "skillNameBg";

// Animations
const char *QSanRoomSkin::S_SKIN_KEY_ANIMATIONS = "preloads";

//ChooseGeneralBox
const char *QSanRoomSkin::S_SKIN_KEY_CHOOSE_GENERAL_BOX_SPLIT_LINE = "chooseGeneralBoxSplitLine";
const char *QSanRoomSkin::S_SKIN_KEY_CHOOSE_GENERAL_BOX_DEST_SEAT = "chooseGeneralBoxDestSeat";

//GeneralCardItem
const char *QSanRoomSkin::S_SKIN_KEY_GENERAL_CARD_ITEM_COMPANION_FONT = "generalCardItemCompanionFont-%1";
const char *QSanRoomSkin::S_SKIN_KEY_GENERAL_CARD_ITEM_COMPANION_ICON = "generalCardItemCompanionIcon";
const char *QSanRoomSkin::S_SKIN_KEY_GENERAL_CARD_ITEM_ARROWS = "generalCardItemArrows-%1";

const char *QSanRoomSkin::S_SKIN_KEY_CARD_ITEM_SMALL_CARDS = "cardItemSmallCards-%1";
const char *QSanRoomSkin::S_SKIN_KEY_CARD_ITEM_CHECK = "cardItemCheck";

QSanSkinFactory *QSanSkinFactory::_sm_singleton = NULL;
QHash<QString, int *> IQSanComponentSkin::QSanSimpleTextFont::_m_fontBank;

IQSanComponentSkin::QSanSimpleTextFont::QSanSimpleTextFont()
{
    memset(this, 0, sizeof(*this));
}

bool IQSanComponentSkin::QSanSimpleTextFont::tryParse(const QVariant &args)
{
    JsonArray arg = args.value<JsonArray>();
    if (arg.size() < 4)
        return false;
    m_vertical = false;
    QString fontPath = arg[0].toString();
    if (fontPath.startsWith("@")) {
        m_vertical = true;
        fontPath.remove(0, 1);
    }
    if (_m_fontBank.contains(fontPath))
        m_fontFace = _m_fontBank[fontPath];
    else {
        m_fontFace = QSanUiUtils::QSanFreeTypeFont::loadFont(fontPath);
        _m_fontBank[fontPath] = m_fontFace;
    }
    if (JsonUtils::isNumber(arg[1])) {
        int size = arg[1].toInt();
        m_fontSize.setWidth(size);
        m_fontSize.setHeight(size);
        m_spacing = 0;
    } else {
        JsonArray arg1 = arg[1].value<JsonArray>();
        m_fontSize.setWidth(arg1[0].toInt());
        m_fontSize.setHeight(arg1[1].toInt());
        m_spacing = arg1[2].toInt();
    }
    m_weight = arg[2].toInt();
    JsonUtils::tryParse(arg[3], m_color);
    return true;
}

bool IQSanComponentSkin::QSanShadowTextFont::tryParse(const QVariant &arg)
{
    if (!arg.isValid() || !arg.canConvert<JsonArray>()) return false;
    JsonArray args = arg.value<JsonArray>();
    if (args.size() < 4) return false;
    if (!QSanSimpleTextFont::tryParse(arg)) return false;
    if (args.size() >= 8) {
        m_shadowRadius = args[4].toInt();
        m_shadowDecadeFactor = args[5].toDouble();
        JsonUtils::tryParse(args[6], m_shadowOffset);

        JsonArray color = args[7].value<JsonArray>();
        m_shadowColor = QColor(color[0].toInt(), color[1].toInt(), color[2].toInt(), color[3].toInt());
    } else {
        m_shadowRadius = -1;
    }
    return true;
}

bool IQSanComponentSkin::isImageKeyDefined(const QString &key) const
{
    const QVariant & val = _m_imageConfig[key];
    return val.canConvert<JsonArray>() || JsonUtils::isString(val);
}

void IQSanComponentSkin::QSanSimpleTextFont::paintText(QPainter *painter, const QRect &pos, Qt::Alignment align,
    const QString &text) const
{
    if (pos.width() <= 0 || pos.height() <= 0 || m_fontSize.width() <= 0 || m_fontSize.height() <= 0) return;
    QSize actualSize = m_fontSize;
    if ((align & Qt::TextWrapAnywhere) && !m_vertical)
        QSanUiUtils::QSanFreeTypeFont::paintQStringMultiLine(painter, text, m_fontFace, m_color,
        actualSize, m_spacing, m_weight, pos, align);
    else
        QSanUiUtils::QSanFreeTypeFont::paintQString(painter, text, m_fontFace, m_color,
        actualSize, m_spacing, m_weight, pos,
        m_vertical ? Qt::Vertical : Qt::Horizontal, align);
}


void IQSanComponentSkin::QSanSimpleTextFont::paintText(QGraphicsPixmapItem *item, const QRect &pos,
    Qt::Alignment align, const QString &text) const
{
    QPixmap pixmap(pos.size());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    paintText(&painter, QRect(0, 0, pos.width(), pos.height()), align, text);
    item->setPixmap(pixmap);
    item->setPos(pos.x(), pos.y());
}

void IQSanComponentSkin::QSanShadowTextFont::paintText(QPainter *painter, const QRect &pos,
    Qt::Alignment align, const QString &text) const
{
    if (pos.width() <= 0 || pos.height() <= 0 || m_fontSize.width() <= 0 || m_fontSize.height() <= 0) return;
    QImage image(pos.width(), pos.height(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    QPainter imagePainter(&image);
    // @todo: currently, we have not considered _m_sahdowOffset yet
    QSanSimpleTextFont::paintText(&imagePainter, QRect(m_shadowRadius, m_shadowRadius,
        pos.width() - m_shadowRadius * 2, pos.height() - m_shadowRadius * 2),
        align, text);
    if (m_shadowRadius < 0 || (m_shadowRadius == 0 && m_shadowOffset.x() == 0 && m_shadowOffset.y() == 0)) {
        painter->drawImage(pos.topLeft(), image);
        return;
    }
    // now, overlay foreground on shadow
    QSanUiUtils::paintShadow(painter, image, m_shadowColor, m_shadowRadius, m_shadowDecadeFactor, pos);
    painter->drawImage(pos.topLeft(), image);
}

void IQSanComponentSkin::QSanShadowTextFont::paintText(QGraphicsPixmapItem *pixmapItem,
    const QRect &pos,
    Qt::Alignment align,
    const QString &text) const
{
    QImage image(pos.width(), pos.height(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    QPainter imagePainter(&image);
    // @todo: currently, we have not considered _m_shadowOffset yet
    QSanSimpleTextFont::paintText(&imagePainter, QRect(m_shadowRadius, m_shadowRadius,
        pos.width() - m_shadowRadius * 2, pos.height() - m_shadowRadius * 2),
        align, text);
    QPixmap pixmap = QPixmap::fromImage(image);
    QPainter shadowPainter(&pixmap);
    QSanUiUtils::paintShadow(&shadowPainter, image, m_shadowColor, m_shadowRadius, m_shadowDecadeFactor, 0, 0);
    shadowPainter.drawImage(0, 0, image);
    pixmapItem->setPixmap(pixmap);
    pixmapItem->setPos(pos.x(), pos.y());
}

QString QSanRoomSkin::getButtonPixmapPath(const QString &groupName,
    const QString &buttonName,
    QSanButton::ButtonState state) const
{
    const char *key;
    QString qkey = QString(QSanRoomSkin::S_SKIN_KEY_BUTTON).arg(groupName);
    QByteArray arr = qkey.toLatin1();
    key = arr.constData();
    if (!isImageKeyDefined(key)) return QString();
    QString path = _m_imageConfig[key].toString();
    QString stateKey;
    if (state == QSanButton::S_STATE_DISABLED)
        stateKey = "disabled";
    else if (state == QSanButton::S_STATE_DOWN)
        stateKey = "down";
    else if (state == QSanButton::S_STATE_HOVER)
        stateKey = "hover";
    else if (state == QSanButton::S_STATE_UP)
        stateKey = "normal";
    else
        return QString();
    return path.arg(buttonName).arg(stateKey);
}

QPixmap QSanRoomSkin::getSkillButtonPixmap(QSanButton::ButtonState state,
    QSanInvokeSkillButton::SkillType type,
    QSanInvokeSkillButton::SkillButtonWidth width) const
{
    QString path = getButtonPixmapPath(S_SKIN_KEY_BUTTON_SKILL,
        QSanInvokeSkillButton::getSkillTypeString(type),
        state);
    if (path.isNull())
        return QPixmap(1, 1); // older Qt version cries for non-zero QPixmap...
    else {
        QString arg2;
        if (width == QSanInvokeSkillButton::S_WIDTH_NARROW)
            arg2 = "3";
        else if (width == QSanInvokeSkillButton::S_WIDTH_MED)
            arg2 = "2";
        else if (width == QSanInvokeSkillButton::S_WIDTH_WIDE)
            arg2 = "1";
        return getPixmapFromFileName(path.arg(arg2));
    }
}

QPixmap QSanRoomSkin::getButtonPixmap(const QString &groupName,
    const QString &buttonName,
    QSanButton::ButtonState state,
    const bool &first_state) const
{
    QString name = buttonName;
    if (!first_state) name += "2";
    QString path = getButtonPixmapPath(groupName, name, state);
    if (path.isNull()) return QPixmap(1, 1); // older Qt version cries for non-zero QPixmap...
    else return getPixmapFromFileName(path);
}

QPixmap QSanRoomSkin::getCardFramePixmap(const QString &frameType) const
{
    return getPixmap(QString(QSanRoomSkin::S_SKIN_KEY_HAND_CARD_FRAME).arg(frameType));
}

QPixmap QSanRoomSkin::getProgressBarPixmap(int percentile) const
{
    QVariant allMaps_var = _m_imageConfig[S_SKIN_KEY_PROGRESS_BAR_IMAGE];
    if (!allMaps_var.canConvert<JsonArray>())
        return QPixmap(1, 1);

    JsonArray allMaps = allMaps_var.value<JsonArray>();

    for (int i = 0; i < allMaps.size(); i++) {
        if (!isNumber(allMaps[i].value<JsonArray>()[0])) continue;
        int thred = allMaps[i].value<JsonArray>()[0].toInt();
        if (thred >= percentile) {
            if (!isString(allMaps[i].value<JsonArray>()[1])) continue;
            return getPixmapFromFileName(allMaps[i].value<JsonArray>()[1].toString(), true);
        }
    }
    return QPixmap(1, 1);
}

bool QSanRoomSkin::generalHasSkin(const QString &general, const int skinId, const bool isCard) const
{
    const QString id = QString::number(skinId);
    QString key;
    if (isCard)
        key = S_SKIN_KEY_GENERAL_CARD;
    else
        key = QString(S_SKIN_KEY_PLAYER_GENERAL_ICON)
        .arg(S_GENERAL_ICON_SIZE_HERO_SKIN);
    QString totalKey = key.arg(id).arg(general);
    if (isImageKeyDefined(totalKey))
        return QFile::exists(_m_imageConfig.value(totalKey).toString());

    totalKey = key.arg(id).arg(S_SKIN_KEY_DEFAULT);
    if (isImageKeyDefined(totalKey))
        return QFile::exists(QString(_m_imageConfig.value(totalKey).toString()).arg(general));

    totalKey = key.arg(S_SKIN_KEY_DEFAULT).arg(S_SKIN_KEY_DEFAULT);
    if (isImageKeyDefined(totalKey))
        return QFile::exists(QString(_m_imageConfig.value(totalKey).toString()).arg(general).arg(id));

    return false;
}

QPixmap QSanRoomSkin::getCardMainPixmap(const QString &cardName) const
{
    if (cardName == "unknown") return getPixmap(S_SKIN_KEY_HAND_CARD_BACK[Config.GeneralLevel]);
    QString name = cardName;
    if (ServerInfo.GameMode == "06_3v3" && name.startsWith("vs_"))
        name = name.mid(3);
    else if (ServerInfo.GameMode == "02_1v1" && name.startsWith("kof_"))
        name = name.mid(4);
    return getPixmap(S_SKIN_KEY_HAND_CARD_MAIN_PHOTO, name);
}

QPixmap QSanRoomSkin::getGeneralCardPixmap(const QString generalName, const int skinId) const
{
    const QString id = QString::number(skinId);
    const QString key = S_SKIN_KEY_GENERAL_CARD;
    if (isImageKeyDefined(key.arg(id).arg(generalName))
        || isImageKeyDefined(key.arg(id).arg(S_SKIN_KEY_DEFAULT))) {
        return getPixmap(key.arg(id), generalName);
    } else {
        return getPixmap(key, generalName, id, true);
    }
}

QPixmap QSanRoomSkin::getCardSuitPixmap(Card::Suit suit) const
{
    return getPixmap(QSanRoomSkin::S_SKIN_KEY_HAND_CARD_SUIT, Card::Suit2String(suit));
}

QPixmap QSanRoomSkin::getCardNumberPixmap(int point, bool isBlack) const
{
    QString pathKey = isBlack ? S_SKIN_KEY_HAND_CARD_NUMBER_BLACK : S_SKIN_KEY_HAND_CARD_NUMBER_RED;
    return getPixmap(pathKey, QString::number(point));
}

QPixmap QSanRoomSkin::getCardJudgeIconPixmap(const QString &judgeName) const
{
    return getPixmap(S_SKIN_KEY_JUDGE_CARD_ICON, judgeName);
}

QPixmap QSanRoomSkin::getCardAvatarPixmap(const QString &generalName) const
{
    QString name = generalName;
    if (ServerInfo.GameMode == "06_3v3" && name.startsWith("vs_"))
        name = name.mid(3);
    else if (ServerInfo.GameMode == "02_1v1" && name.startsWith("kof_"))
        name = name.mid(4);
    return getGeneralPixmap(name, S_GENERAL_ICON_SIZE_TINY);
}

QPixmap QSanRoomSkin::getGeneralPixmap(const QString &generalName, GeneralIconSize size, const int skinId) const
{
    QString name = generalName;
    if (ServerInfo.GameMode == "06_3v3" && name.startsWith("vs_"))
        name = name.mid(3);
    else if (ServerInfo.GameMode == "02_1v1" && name.startsWith("kof_"))
        name = name.mid(4);
    if (size == S_GENERAL_ICON_SIZE_CARD)
        return getGeneralCardPixmap(generalName, skinId);

    QString id = QString::number(skinId);
    QString key = QString(S_SKIN_KEY_PLAYER_GENERAL_ICON)
        .arg(size).arg(id).arg(name);
    if (isImageKeyDefined(key)) //exactly match
        return getPixmap(key);

    key = QString(S_SKIN_KEY_PLAYER_GENERAL_ICON)
        .arg(size).replace("%3", name);
    if (isImageKeyDefined(key.arg(S_SKIN_KEY_DEFAULT))) //try matching name and size
        return getPixmap(key, id);

    key = QString(S_SKIN_KEY_PLAYER_GENERAL_ICON)
        .arg(size).arg(id);
    if (isImageKeyDefined(key.arg(S_SKIN_KEY_DEFAULT))) //try matching size and id
        return getPixmap(key, name);

    key = QString(S_SKIN_KEY_PLAYER_GENERAL_ICON) //try the default match
        .arg(size);
    return getPixmap(key, name, id, true);
}

QString QSanRoomSkin::getPlayerAudioEffectPath(const QString &eventName, const QString &category, int index, const Player *player) const
{
    QString fileName;
    QString key = QString(QSanRoomSkin::S_SKIN_KEY_PLAYER_AUDIO_EFFECT).arg(category).arg(eventName);

    if (index < 0)
        fileName = getRandomAudioFileName(key);
    else {
        QStringList fileNames = getAudioFileNames(key);
        if (!fileNames.isEmpty()) {
            if (fileNames.length() >= index)
                return fileNames[index - 1];
            else {
                while (index > fileNames.length())
                    index -= fileNames.length();
                return fileNames[index - 1];
            }
        }
    }

    if (fileName.isEmpty()) {
        QStringList name_list = eventName.split("-");
        QString skillName, generalName;
        if (name_list.length() == 2){
            generalName = name_list.first();
            skillName = name_list.last();
        }else {
            skillName = eventName;
            generalName = QString();
        }

        int skinId = 0;
        if (!generalName.isEmpty() && player != NULL) {
            if (player->getGeneralName() == generalName)
                skinId = player->getHeadSkinId();
            else if (player->getGeneral2Name() == generalName)
                skinId = player->getDeputySkinId();
        }

        QStringList fileNames = Sanguosha->findSkillAudioFileNames(skillName, generalName, skinId);

        if (!fileNames.isEmpty()) {
            if (index < 0)
                fileName = fileNames.at(qrand() % fileNames.length());
            else {
                if (fileNames.length() >= index)
                    return fileNames[index - 1];
                else {
                    while (index > fileNames.length())
                        index -= fileNames.length();
                    return fileNames[index - 1];
                }
            }
        }
    }

    if (fileName.isEmpty())
        fileName = _m_audioConfig[QString(S_SKIN_KEY_PLAYER_AUDIO_EFFECT).arg(category).arg("default").toLatin1().constData()].toString().arg(eventName);
    
    return fileName;
}

QString QSanRoomSkin::getPlayerAudioEffectPath(const QString &eventName, bool isMale, int index) const
{
    return getPlayerAudioEffectPath(eventName, QString(isMale ? "male" : "female"), index);
}

QRect IQSanComponentSkin::AnchoredRect::getTranslatedRect(QRect parentRect, QSize size) const
{
    QPoint parentAnchor;
    Qt::Alignment hAlign = m_anchorParent & Qt::AlignHorizontal_Mask;
    if (hAlign & Qt::AlignRight)
        parentAnchor.setX(parentRect.right());
    else if (hAlign & Qt::AlignHCenter)
        parentAnchor.setX(parentRect.center().x());
    else
        parentAnchor.setX(parentRect.left());
    Qt::Alignment vAlign = m_anchorParent & Qt::AlignVertical_Mask;
    if (vAlign & Qt::AlignBottom)
        parentAnchor.setY(parentRect.bottom());
    else if (vAlign & Qt::AlignVCenter)
        parentAnchor.setY(parentRect.center().y());
    else
        parentAnchor.setY(parentRect.top());

    QPoint childAnchor;
    hAlign = m_anchorChild & Qt::AlignHorizontal_Mask;
    if (hAlign & Qt::AlignRight)
        childAnchor.setX(size.width());
    else if (hAlign & Qt::AlignHCenter)
        childAnchor.setX(size.width() / 2);
    else
        childAnchor.setX(0);
    vAlign = m_anchorChild & Qt::AlignVertical_Mask;
    if (vAlign & Qt::AlignBottom)
        childAnchor.setY(size.height());
    else if (vAlign & Qt::AlignVCenter)
        childAnchor.setY(size.height() / 2);
    else
        childAnchor.setY(0);

    QPoint pos = parentAnchor - childAnchor + m_offset;
    QRect rect(pos, size);
    return rect;
}

QRect IQSanComponentSkin::AnchoredRect::getTranslatedRect(QRect parentRect) const
{
    Q_ASSERT(m_useFixedSize);
    return getTranslatedRect(parentRect, m_fixedSize);
}

bool IQSanComponentSkin::AnchoredRect::tryParse(const QVariant &var)
{
    // must be in one of the following format:
    // [offsetX, offestY, sizeX, sizeY]
    // [childAnchor, parentAnchor, [offsetX, offsetY]]
    // [childAnchor, parentAnchor, [offsetX, offsetY], [sizeX, sizeY]]
    if (!var.canConvert<JsonArray>()) return false;
    m_useFixedSize = false;
    m_anchorChild = m_anchorParent = Qt::AlignLeft | Qt::AlignTop;
    JsonArray value = var.value<JsonArray>();
    if (JsonUtils::isNumberArray(value, 0, 3)) {
        QRect rect;
        bool success = JsonUtils::tryParse(value, rect);
        if (!success) return false;
        m_useFixedSize = true;
        m_fixedSize = rect.size();
        m_offset = rect.topLeft();
    } else if (JsonUtils::isStringArray(value, 0, 0) && value.size() >= 3 && JsonUtils::isNumberArray(value[2], 0, 1)) {
        if (JsonUtils::tryParse(value[0], m_anchorChild)
            && JsonUtils::tryParse(value[1], m_anchorParent)
            && JsonUtils::tryParse(value[2], m_offset)) {
            if (value.size() >= 4 && JsonUtils::isNumberArray(value[3].value<JsonArray>(), 0, 1)
                && JsonUtils::tryParse(value[3], m_fixedSize))
                m_useFixedSize = true;
            return true;
        }
    }
    return false;
}

// Load pixmap from a file and map it to the given key.
QPixmap QSanPixmapCache::getPixmap(const QString &key, const QString &fileName)
{
    static QPixmap empty;
    QPixmap pixmap;
    if (QPixmapCache::find(key, &pixmap)) {
        return pixmap;
    }

    if (fileName == "deprecated") {
        QPixmapCache::insert(key, empty);
    } else {
        bool success = !fileName.isEmpty() && pixmap.load(fileName);
        if (!success) {
            /*qWarning("Unable to open resource file \"%s\" for key \"%s\"\n",
                     fileName.toLatin1().constData(),
                     key.toLatin1().constData());*/
            QPixmapCache::insert(key, empty); // make Qt happy
        } else {
            QPixmapCache::insert(key, pixmap);
            return pixmap;
        }
    }

    return empty;
}

// Load pixmap from a file.
QPixmap QSanPixmapCache::getPixmap(const QString &fileName)
{
    return getPixmap(fileName, fileName);
}

bool QSanPixmapCache::contains(const QString &key)
{
    return QPixmapCache::find(key);
}

bool IQSanComponentSkin::_loadImageConfig(const QVariant &config)
{
    if (!config.canConvert<JsonObject>())
        return false;
    if (_m_imageConfig.isEmpty())
        _m_imageConfig = config.value<JsonObject>();
    else {
        JsonObject object = config.value<JsonObject>();
        const QList<QString> &keys = object.keys();
        foreach (const QString &key, keys) {
            _m_imageConfig[key] = object[key];
            S_IMAGE_KEY2FILE.remove(key);
            S_IMAGE_KEY2PIXMAP.remove(key);
            if (S_IMAGE_GROUP_KEYS.contains(key)) {
                const QList<QString> &mappedKeys = S_IMAGE_GROUP_KEYS[key];
                foreach (const QString &mkey, mappedKeys) {
                    S_IMAGE_KEY2FILE.remove(mkey);
                    S_IMAGE_KEY2PIXMAP.remove(mkey);
                }
                S_IMAGE_GROUP_KEYS.remove(key);
            }
        }
    }
    return true;
}

bool IQSanComponentSkin::load(const QString &layoutConfigName, const QString &imageConfigName,
    const QString &audioConfigName, const QString &animationConfigName)
{
    bool success = true;
    QString errorMsg;

    if (!layoutConfigName.isNull()) {
        JsonDocument layoutDoc = JsonDocument::fromFilePath(layoutConfigName);
        if (!layoutDoc.isValid() || !layoutDoc.isObject()) {
            errorMsg = QString("Error when reading layout config file \"%1\": \n%2")
                .arg(layoutConfigName).arg(layoutDoc.errorString());
            QMessageBox::warning(NULL, "Config Error", errorMsg);
            success = false;
        }
        success = _loadLayoutConfig(layoutDoc.toVariant());
    }

    if (!imageConfigName.isNull()) {
        JsonDocument imageDoc = JsonDocument::fromFilePath(imageConfigName);
        if (!imageDoc.isValid() || !imageDoc.isObject()) {
            errorMsg = QString("Error when reading image config file \"%1\": \n%2")
                .arg(imageConfigName).arg(imageDoc.errorString());
            QMessageBox::warning(NULL, "Config Error", errorMsg);
            success = false;
        }
        success = _loadImageConfig(imageDoc.toVariant());
    }

    if (!audioConfigName.isNull()) {
        JsonDocument audioDoc = JsonDocument::fromFilePath(audioConfigName);
        if (!audioDoc.isValid() || !audioDoc.isObject()) {
            errorMsg = QString("Error when reading audio config file \"%1\": \n%2")
                .arg(audioConfigName).arg(audioDoc.errorString());
            QMessageBox::warning(NULL, "Config Error", errorMsg);
            success = false;
        }
        _m_audioConfig = audioDoc.object();
    }

    if (!animationConfigName.isNull()) {
        JsonDocument animDoc = JsonDocument::fromFilePath(animationConfigName);
        if (!animDoc.isValid() || !animDoc.isObject()) {
            errorMsg = QString("Error when reading animation config file \"%1\": \n%2")
                .arg(animationConfigName).arg(animDoc.errorString());
            QMessageBox::warning(NULL, "Config Error", errorMsg);
            success = false;
        }
        _m_animationConfig = animDoc.object();
    }

    return success;
}

QStringList IQSanComponentSkin::getAudioFileNames(const QString &key) const
{
    const QVariant &result = _m_audioConfig[key];
    if (result.isNull())
        return QStringList();
    else if (JsonUtils::isString(result))
        return QStringList(result.toString());
    else if (result.canConvert<JsonArray>()) {
        QStringList audios;
        tryParse(result, audios);
        return audios;
    }
    return QStringList();
}

QStringList IQSanComponentSkin::getAnimationFileNames() const
{
    QStringList animations;

    const QVariant &result = _m_animationConfig[QSanRoomSkin::S_SKIN_KEY_ANIMATIONS];
    tryParse(result, animations);
    return animations;
}

QString IQSanComponentSkin::getRandomAudioFileName(const QString &key) const
{
    QStringList audios = getAudioFileNames(key);
    if (audios.isEmpty()) return QString();
    int r = qrand() % audios.length();
    return audios[r];
}

QString IQSanComponentSkin::_readConfig(const QVariant &dict, const QString &key,
    const QString &defaultValue) const
{
    if (!dict.canConvert<JsonObject>()) return defaultValue;
    JsonObject dictMap = dict.value<JsonObject>();
    const QVariant &val = dictMap[key];
    if (!JsonUtils::isString(val)) {
        qWarning("Unable to read configuration: %s", key.toLatin1().constData());
        return defaultValue;
    } else {
        return val.toString();
    }
}

QString IQSanComponentSkin::_readImageConfig(const QString &key, QRect &rect,
    bool &clipping, QSize &newScale,
    bool &scaled, const QString &defaultValue) const
{
    clipping = false;
    scaled = false;
    if (_m_imageConfig.isEmpty())
        return defaultValue;
    const QVariant &val = _m_imageConfig[key];
    QString result;
    if (JsonUtils::isString(val)) {
        result = val.toString();
    } else if (val.canConvert<JsonArray>()) {
        JsonArray arr = val.value<JsonArray>();
        if (arr.size() >= 2 && JsonUtils::isString(arr[0]) && tryParse(arr[1], rect)) {
            clipping = true;
            result = arr[0].toString();
            if (arr.size() > 3 && tryParse(arr[3], newScale))
                scaled = true;
        }
    } else {
        qWarning("Unable to read configuration: %s", key.toLatin1().constData());
        return defaultValue;
    }
    return result;
}

QHash<QString, QString> IQSanComponentSkin::S_IMAGE_KEY2FILE;
QHash<QString, QList<QString> > IQSanComponentSkin::S_IMAGE_GROUP_KEYS;
QHash<QString, QPixmap> IQSanComponentSkin::S_IMAGE_KEY2PIXMAP;
QHash<QString, int> IQSanComponentSkin::S_HERO_SKIN_INDEX;

QPixmap IQSanComponentSkin::getPixmap(const QString &key, const QString &arg, const QString &arg2, bool addDefaultArg) const
{
    // the order of attempts are:
    // 1. if no arg, then just use key to find fileName.
    // 2. try key.arg(arg), if exists, then return the pixmap
    // 3. try key.arg(default), get fileName, and try fileName.arg(arg).arg(arg2) or fileName.arg(arg) if no arg2
    QString totalKey;
    QString groupKey;
    QString fileName;
    QRect clipRegion;
    QSize scaleRegion;
    bool clipping = false;
    bool scaled = false;

    QString initialKey = key;
    QString cacheKey;

    if (addDefaultArg)
        initialKey = initialKey.arg(S_SKIN_KEY_DEFAULT);

    // case 1 and 2
    if (arg.isNull())
        totalKey = initialKey;
    else
        totalKey = initialKey.arg(arg);

    cacheKey = totalKey;
    if (addDefaultArg && !arg2.isNull())
        cacheKey = key.arg(arg).arg(arg2);

    //bool from_cache = false;
    if (S_IMAGE_KEY2FILE.contains(cacheKey)) { // first, search cache
        fileName = S_IMAGE_KEY2FILE[cacheKey];
        //from_cache = true;
    } else if (isImageKeyDefined(totalKey)) { // then, read from config file
        fileName = _readImageConfig(totalKey, clipRegion, clipping, scaleRegion, scaled);
        S_IMAGE_KEY2FILE[cacheKey].append(fileName);
    } else { // case 3: use default
        //Q_ASSERT(!arg.isNull());
        groupKey = initialKey.arg(S_SKIN_KEY_DEFAULT);
        S_IMAGE_GROUP_KEYS[groupKey].append(totalKey);
        QString fileNameToResolve = _readImageConfig(groupKey, clipRegion, clipping, scaleRegion, scaled);
        fileName = fileNameToResolve.arg(arg);
        if (!QFile::exists(fileName)) {
            if (!arg2.isNull() && fileName.contains("%2"))
                fileName = fileName.arg(arg2);
            if (!QFile::exists(fileName)) {
                groupKey = initialKey.arg(S_SKIN_KEY_DEFAULT_SECOND);
                S_IMAGE_GROUP_KEYS[groupKey].append(totalKey);
                QString fileNameToResolve = _readImageConfig(groupKey, clipRegion, clipping, scaleRegion, scaled);
                fileName = fileNameToResolve.arg(arg);
                if (!QFile::exists(fileName) && !arg2.isNull() && fileName.contains("%2"))
                    fileName = fileName.arg(arg2);
            }
        }
    }

    if (!S_IMAGE_KEY2PIXMAP.contains(cacheKey)) {
        QPixmap pixmap = QSanPixmapCache::getPixmap(fileName);
        if (clipping) {
            QRect actualClip = clipRegion;
            if (actualClip.right() > pixmap.width())
                actualClip.setRight(pixmap.width());
            if (actualClip.bottom() > pixmap.height())
                actualClip.setBottom(pixmap.height());

            QPixmap clipped = QPixmap(clipRegion.size());
            clipped.fill(Qt::transparent);
            QPainter painter(&clipped);
            painter.drawPixmap(0, 0, pixmap.copy(actualClip));

            if (scaled)
                clipped = clipped.scaled(scaleRegion, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            pixmap = clipped;
        } else if (pixmap.isNull()) {
            if (scaleRegion.isValid())
                pixmap = QPixmap(scaleRegion);
            else if (clipRegion.isValid())
                pixmap = QPixmap(clipRegion.size());
            else
                pixmap = QPixmap(1, 1);
            pixmap.fill();
        }
        S_IMAGE_KEY2PIXMAP[cacheKey] = pixmap;
    }
    return S_IMAGE_KEY2PIXMAP[cacheKey];
}

QPixmap IQSanComponentSkin::getPixmapFileName(const QString &key) const
{
    return _readConfig(_m_imageConfig, key);
}

QPixmap IQSanComponentSkin::getPixmapFromFileName(const QString &fileName, bool cache) const
{
    if (fileName == "deprecated" || fileName.isEmpty()) {
        return QPixmap(1, 1);
    } else {
        QPixmap pixmap;
        bool success = true;
        if (!cache) {
            success = pixmap.load(fileName);
        } else {
            if (!QPixmapCache::find(fileName, &pixmap)) {
                if (pixmap.load(fileName))
                    QPixmapCache::insert(fileName, pixmap);
                else
                    success = false;
            }
        }
        if (!success) return QPixmap(1, 1); // make Qt happy
        else return pixmap;
    }
}

bool QSanRoomSkin::_loadAnimationConfig(const QVariant &)
{
    return true;
}

QAbstractAnimation *QSanRoomSkin::createHuaShenAnimation(QPixmap &huashenAvatar, QPoint topLeft, QGraphicsItem *parent,
    QGraphicsItem *&huashenAvatarCreated) const
{
    QLabel *avatar = new QLabel;
    avatar->setStyleSheet("QLabel { background-color: transparent; }");
    avatar->setPixmap(huashenAvatar);
    QGraphicsProxyWidget *widget = new QGraphicsProxyWidget(parent);
    widget->setWidget(avatar);
    widget->setPos(topLeft);

    QPropertyAnimation *animation = new QPropertyAnimation(widget, "opacity");
    animation->setLoopCount(2000);
    JsonArray huashenConfig = _m_animationConfig["huashen"].value<JsonArray>();
    int duration;
    if (tryParse(huashenConfig[0], duration) && huashenConfig[1].canConvert<JsonArray>()) {
        animation->setDuration(duration);
        JsonArray keyValues = huashenConfig[1].value<JsonArray>();
        for (int i = 0; i < keyValues.size(); i++) {
            QVariant keyValue = keyValues[i];
            if (!keyValue.canConvert<JsonArray>() || keyValue.value<JsonArray>().length() != 2) continue;
            double step;
            double val;
            JsonArray keyArr = keyValue.value<JsonArray>();
            if (!tryParse(keyArr[0], step) || !tryParse(keyArr[1], val)) continue;
            animation->setKeyValueAt(step, val);
        }
    }
    huashenAvatarCreated = widget;
    return animation;
}

const QSanRoomSkin::RoomLayout &QSanRoomSkin::getRoomLayout() const
{
    return this->_m_roomLayout;
}

const QSanRoomSkin::PhotoLayout &QSanRoomSkin::getPhotoLayout() const
{
    return _m_photoLayout;
}

const QSanRoomSkin::DashboardLayout &QSanRoomSkin::getDashboardLayout() const
{
    return _m_dashboardLayout;
}

const QSanRoomSkin::CommonLayout &QSanRoomSkin::getCommonLayout() const
{
    return _m_commonLayout;
}

QSanRoomSkin::QSanShadowTextFont
QSanRoomSkin::DashboardLayout::getSkillTextFont(QSanButton::ButtonState state,
QSanInvokeSkillButton::SkillType type,
QSanInvokeSkillButton::SkillButtonWidth width) const
{
    int i = QSanButton::S_NUM_BUTTON_STATES * (int)type + (int)state;
    QSanShadowTextFont font = m_skillTextFonts[width];
    font.m_color = m_skillTextColors[i];
    font.m_shadowColor = m_skillTextShadowColors[i];
    return font;
}

bool QSanRoomSkin::_loadLayoutConfig(const QVariant &layout)
{
    JsonObject layoutConfig = layout.value<JsonObject>();
    JsonObject config = layoutConfig[S_SKIN_KEY_COMMON].value<JsonObject>();
    tryParse(config["cardNormalHeight"], _m_commonLayout.m_cardNormalHeight);
    tryParse(config["cardNormalWidth"], _m_commonLayout.m_cardNormalWidth);
    tryParse(config["hpExtraSpaceHolder"], _m_commonLayout.m_hpExtraSpaceHolder);
    tryParse(config["cardMainArea"], _m_commonLayout.m_cardMainArea);
    tryParse(config["cardSuitArea"], _m_commonLayout.m_cardSuitArea);
    tryParse(config["validateSuitArea"], _m_commonLayout.m_validateSuitArea);
    tryParse(config["cardNumberArea"], _m_commonLayout.m_cardNumberArea);
    tryParse(config["cardFrameArea"], _m_commonLayout.m_cardFrameArea);
    tryParse(config["cardFootnoteArea"], _m_commonLayout.m_cardFootnoteArea);
    tryParse(config["cardAvatarArea"], _m_commonLayout.m_cardAvatarArea);
    tryParse(config["chooseGeneralBoxSwitchIconSizeThreshold"],
        _m_commonLayout.m_chooseGeneralBoxSwitchIconSizeThreshold);
    tryParse(config["chooseGeneralBoxSwitchIconEachRow"],
        _m_commonLayout.m_chooseGeneralBoxSwitchIconEachRow);
    tryParse(config["chooseGeneralBoxSwitchIconEachRowForTooManyGenerals"],
        _m_commonLayout.m_chooseGeneralBoxSwitchIconEachRowForTooManyGenerals);
    tryParse(config["chooseGeneralBoxNoIconThreshold"],
        _m_commonLayout.m_chooseGeneralBoxNoIconThreshold);
    tryParse(config["chooseGeneralBoxDenseIconSize"],
        _m_commonLayout.m_chooseGeneralBoxDenseIconSize);
    tryParse(config["chooseGeneralBoxSparseIconSize"],
        _m_commonLayout.m_chooseGeneralBoxSparseIconSize);
    tryParse(config["tinyAvatarSize"], _m_commonLayout.m_tinyAvatarSize);
    tryParse(config["bubbleChatBoxShowAreaSize"],
        _m_commonLayout.m_bubbleChatBoxShowAreaSize);
    _m_commonLayout.m_cardFootnoteFont.tryParse(config["cardFootnoteFont"]);

    tryParse(config["promptInfoSize"], _m_commonLayout.m_promptInfoSize);
    _m_commonLayout.m_promptInfoFont.tryParse(config["promptInfoFont"]);

    _m_commonLayout.m_choiceInfoFont.tryParse(config["choiceInfoFont"]);

    JsonArray magatamaFont = config["magatamaFont"].value<JsonArray>();
    for (int i = 0; i < 6 && i < magatamaFont.size(); i++) {
        _m_commonLayout.m_hpFont[i].tryParse(magatamaFont[i]);
    }

    tryParse(config["graphicsBoxBgColor"], _m_commonLayout.graphicsBoxBackgroundColor);
    tryParse(config["graphicsBoxBorderColor"], _m_commonLayout.graphicsBoxBorderColor);
    _m_commonLayout.graphicsBoxTitleFont.tryParse(config["graphicsBoxTitleFont"]);

    _m_commonLayout.m_chooseGeneralBoxDestSeatFont.tryParse(config["generalBoxDestSeatFont"]);

    tryParse(config["generalCardItemCompanionPromptRegion"], _m_commonLayout.m_generalCardItemCompanionPromptRegion);

    _m_commonLayout.optionButtonText.tryParse(config["optionButtonText"]);

    tryParse(config["generalButtonPositionIconRegion"], _m_commonLayout.generalButtonPositionIconRegion);
    tryParse(config["generalButtonNameRegion"], _m_commonLayout.generalButtonNameRegion);

    _m_commonLayout.playerCardBoxPlaceNameText.tryParse(config["playerCardBoxPlaceNameText"]);
    _m_commonLayout.skinItemTitleText.tryParse(config["skinItemTitleText"]);

    config = layoutConfig[S_SKIN_KEY_ROOM].value<JsonObject>();
    tryParse(config["chatBoxHeightPercentage"], _m_roomLayout.m_chatBoxHeightPercentage);
    tryParse(config["chatTextBoxHeight"], _m_roomLayout.m_chatTextBoxHeight);
    tryParse(config["discardPileMinWidth"], _m_roomLayout.m_discardPileMinWidth);
    tryParse(config["discardPilePadding"], _m_roomLayout.m_discardPilePadding);
    tryParse(config["infoPlaneWidthPercentage"], _m_roomLayout.m_infoPlaneWidthPercentage);
    tryParse(config["logBoxHeightPercentage"], _m_roomLayout.m_logBoxHeightPercentage);
    tryParse(config["minimumSceneSize"], _m_roomLayout.m_minimumSceneSize);
    tryParse(config["maximumSceneSize"], _m_roomLayout.m_maximumSceneSize);
    tryParse(config["minimumSceneSize-10player"], _m_roomLayout.m_minimumSceneSize10Player);
    tryParse(config["maximumSceneSize-10player"], _m_roomLayout.m_maximumSceneSize10Player);
    tryParse(config["photoHDistance"], _m_roomLayout.m_photoHDistance);
    tryParse(config["photoVDistance"], _m_roomLayout.m_photoVDistance);
    tryParse(config["photoDashboardPadding"], _m_roomLayout.m_photoDashboardPadding);
    tryParse(config["photoRoomPadding"], _m_roomLayout.m_photoRoomPadding);
    tryParse(config["roleBoxHeight"], _m_roomLayout.m_roleBoxHeight);
    tryParse(config["scenePadding"], _m_roomLayout.m_scenePadding);

    for (int i = 0; i < 2; i++) {
        JsonObject playerConfig;
        PlayerCardContainerLayout *layout;
        if (i == 0) {
            layout = &_m_photoLayout;
            playerConfig = layoutConfig[S_SKIN_KEY_PHOTO].value<JsonObject>();
        } else {
            layout = &_m_dashboardLayout;
            playerConfig = layoutConfig[S_SKIN_KEY_DASHBOARD].value<JsonObject>();
        }

        tryParse(playerConfig["normalHeight"], layout->m_normalHeight);
        tryParse(playerConfig["handCardNumIconArea"], layout->m_handCardArea);
        tryParse(playerConfig["handPileNumTextArea"], layout->m_handPileArea);
        JsonArray equipAreas = playerConfig["equipAreas"].value<JsonArray>();
        for (int j = 0; j < S_EQUIP_AREA_LENGTH && j < equipAreas.size(); j++)
            tryParse(equipAreas[j], layout->m_equipAreas[j]);
        JsonArray horseAreas = playerConfig["horseAreas"].value<JsonArray>();
        for (int j = 0; j < S_HORSE_AREA_LENGTH && j < horseAreas.size(); j++)
            tryParse(horseAreas[j], layout->m_horseAreas[j]);
        tryParse(playerConfig["equipImageArea"], layout->m_equipImageArea);
        tryParse(playerConfig["equipSuitArea"], layout->m_equipSuitArea);
        tryParse(playerConfig["equipPointArea"], layout->m_equipPointArea);
        tryParse(playerConfig["horseImageArea"], layout->m_horseImageArea);
        tryParse(playerConfig["horseSuitArea"], layout->m_horseSuitArea);
        tryParse(playerConfig["horsePointArea"], layout->m_horsePointArea);
        if (!layout->m_equipPointFontBlack.tryParse(playerConfig["equipPointFontBlack"]))
            layout->m_equipPointFontBlack.tryParse(playerConfig["equipPointFont"]);
        if (!layout->m_equipPointFontRed.tryParse(playerConfig["equipPointFontRed"]))
            layout->m_equipPointFontRed.tryParse(playerConfig["equipPointFont"]);

        tryParse(playerConfig["delayedTrickFirstRegion"], layout->m_delayedTrickFirstRegion);
        tryParse(playerConfig["delayedTrickStep"], layout->m_delayedTrickStep);

        layout->m_markTextArea.tryParse(playerConfig["markTextArea"]);
        tryParse(playerConfig["roleComboBoxPos"], layout->m_roleComboBoxPos);

        tryParse(playerConfig["avatarArea"], layout->m_avatarArea);
        tryParse(playerConfig["secondaryAvatarArea"], layout->m_smallAvatarArea);
        tryParse(playerConfig["circleArea"], layout->m_circleArea);
        tryParse(playerConfig["avatarImageType"], layout->m_avatarSize);
        tryParse(playerConfig["secondaryAvatarImageType"], layout->m_smallAvatarSize);
        tryParse(playerConfig["primaryAvatarImageType"], layout->m_primaryAvatarSize);
        tryParse(playerConfig["circleImageType"], layout->m_circleImageSize);
        tryParse(playerConfig["avatarNameArea"], layout->m_avatarNameArea);
        layout->m_avatarNameFont.tryParse(playerConfig["avatarNameFont"]);
        tryParse(playerConfig["smallAvatarNameArea"], layout->m_smallAvatarNameArea);
        layout->m_smallAvatarNameFont.tryParse(playerConfig["smallAvatarNameFont"]);
        tryParse(playerConfig["kingdomMaskArea"], layout->m_kingdomMaskArea);
        tryParse(playerConfig["kingdomIconArea"], layout->m_kingdomIconArea);

        layout->m_handCardFont.tryParse(playerConfig["handCardFont"]);
        layout->m_handPileFont.tryParse(playerConfig["handPileFont"]);
        tryParse(playerConfig["screenNameArea"], layout->m_screenNameArea);
        layout->m_screenNameFont.tryParse(playerConfig["screenNameFont"]);

        layout->m_progressBarArea.tryParse(playerConfig["progressBarArea"]);
        tryParse(playerConfig["progressBarHorizontal"], layout->m_isProgressBarHorizontal);
        tryParse(playerConfig["magatamaSize"], layout->m_magatamaSize);
        tryParse(playerConfig["magatamaImageArea"], layout->m_magatamaImageArea);
        tryParse(playerConfig["magatamaNumberArea"], layout->m_magatamaNumberArea);
        tryParse(playerConfig["magatamasBgVisible"], layout->m_magatamasBgVisible);
        tryParse(playerConfig["magatamasSpacing"], layout->m_magatamasSpacing);
        tryParse(playerConfig["magatamasOffset"], layout->m_magatamasOffset);
        JsonArray magatamasAnchor = playerConfig["magatamasAnchor"].value<JsonArray>();
        if (!magatamasAnchor.isEmpty()) {
            if (magatamasAnchor.size() > 1)
                tryParse(magatamasAnchor[1], layout->m_magatamasAnchor);
            if (JsonUtils::isString(magatamasAnchor[0]))
                tryParse(magatamasAnchor[0], layout->m_magatamasAlign);
        }

        layout->m_phaseArea.tryParse(playerConfig["phaseArea"]);
        tryParse(playerConfig["privatePileStartPos"], layout->m_privatePileStartPos);
        tryParse(playerConfig["privatePileStep"], layout->m_privatePileStep);
        tryParse(playerConfig["privatePileButtonSize"], layout->m_privatePileButtonSize);
        tryParse(playerConfig["actionedIconRegion"], layout->m_actionedIconRegion);
        tryParse(playerConfig["saveMeIconRegion"], layout->m_saveMeIconRegion);
        tryParse(playerConfig["chainedIconRegion"], layout->m_chainedIconRegion);
        layout->m_deathIconRegion.tryParse(playerConfig["deathIconRegion"]);
        tryParse(playerConfig["votesIconRegion"], layout->m_votesIconRegion);
        tryParse(playerConfig["seatIconRegion"], layout->m_seatIconRegion);
        tryParse(playerConfig["drankMaskColor"], layout->m_drankMaskColor);
        tryParse(playerConfig["duanchangMaskColor"], layout->m_duanchangMaskColor);
        tryParse(playerConfig["deathEffectColor"], layout->m_deathEffectColor);
        tryParse(playerConfig["extraSkillArea"], layout->m_extraSkillArea);
        layout->m_extraSkillFont.tryParse(playerConfig["extraSkillFont"]);
        tryParse(playerConfig["extraSkillTextArea"], layout->m_extraSkillTextArea);
    }


    config = layoutConfig[S_SKIN_KEY_PHOTO].value<JsonObject>();

    tryParse(config["normalWidth"], _m_photoLayout.m_normalWidth);
    if (!tryParse(config["focusFrameArea"], _m_photoLayout.m_focusFrameArea)
        && isNumber(config["borderWidth"])) {
        int borderWidth = 0;
        tryParse(config["borderWidth"], borderWidth);
        _m_photoLayout.m_focusFrameArea = QRect(-borderWidth, -borderWidth,
            _m_photoLayout.m_normalWidth + 2 * borderWidth,
            _m_photoLayout.m_normalHeight + 2 * borderWidth);
    }
    tryParse(config["mainFrameArea"], _m_photoLayout.m_mainFrameArea);
    tryParse(config["onlineStatusArea"], _m_photoLayout.m_onlineStatusArea);
    tryParse(config["onlineStatusBgColor"], _m_photoLayout.m_onlineStatusBgColor);
    _m_photoLayout.m_onlineStatusFont.tryParse(config["onlineStatusFont"]);
    tryParse(config["cardMoveArea"], _m_photoLayout.m_cardMoveRegion);
    tryParse(config["skillNameArea"], _m_photoLayout.m_skillNameArea);
    _m_photoLayout.m_skillNameFont.tryParse(config["skillNameFont"]);
    tryParse(config["canvasArea"], _m_photoLayout.m_boundingRect);

    config = layoutConfig[S_SKIN_KEY_DASHBOARD].value<JsonObject>();
    tryParse(config["leftWidth"], _m_dashboardLayout.m_leftWidth);
    tryParse(config["rightWidth"], _m_dashboardLayout.m_rightWidth);
    tryParse(config["floatingAreaHeight"], _m_dashboardLayout.m_floatingAreaHeight);
    tryParse(config["focusFrameArea"], _m_dashboardLayout.m_focusFrameArea);
    tryParse(config["buttonSetSize"], _m_dashboardLayout.m_buttonSetSize);
    tryParse(config["buttonSetSizeOld"], _m_dashboardLayout.m_buttonSetSizeOld);
    tryParse(config["confirmButtonArea"], _m_dashboardLayout.m_confirmButtonArea);
    tryParse(config["cancelButtonArea"], _m_dashboardLayout.m_cancelButtonArea);
    tryParse(config["discardButtonArea"], _m_dashboardLayout.m_discardButtonArea);
    tryParse(config["trustButtonArea"], _m_dashboardLayout.m_trustButtonArea);
    tryParse(config["confirmButtonAreaOld"], _m_dashboardLayout.m_confirmButtonAreaOld);
    tryParse(config["cancelButtonAreaOld"], _m_dashboardLayout.m_cancelButtonAreaOld);
    tryParse(config["discardButtonAreaOld"], _m_dashboardLayout.m_discardButtonAreaOld);
    tryParse(config["equipBorderPos"], _m_dashboardLayout.m_equipBorderPos);
    tryParse(config["equipSelectedOffset"], _m_dashboardLayout.m_equipSelectedOffset);
    tryParse(config["equipEnabledOffset"], _m_dashboardLayout.m_equipEnabledOffset);
    tryParse(config["disperseWidth"], _m_dashboardLayout.m_disperseWidth);
    tryParse(config["trustEffectColor"], _m_dashboardLayout.m_trustEffectColor);
    tryParse(config["changeHeadHeroSkinBtnPos"],
        _m_dashboardLayout.m_changeHeadHeroSkinButtonPos);
    tryParse(config["changeDeputyHeroSkinBtnPos"],
        _m_dashboardLayout.m_changeDeputyHeroSkinButtonPos);
    tryParse(config["skillDockLeftMargin"], _m_dashboardLayout.m_skillDockLeftMargin);
    tryParse(config["skillDockRightMargin"], _m_dashboardLayout.m_skillDockRightMargin);
    tryParse(config["skillDockBottomMargin"], _m_dashboardLayout.m_skillDockBottomMargin);
    config = layoutConfig["skillButton"].value<JsonObject>();
    JsonArray configWidth = config["width"].value<JsonArray>();
    JsonArray configTextArea = config["textArea"].value<JsonArray>();
    JsonArray configTextAreaDown = config["textAreaDown"].value<JsonArray>();
    JsonArray configTextFont = config["textFont"].value<JsonArray>();
    for (int i = 0; i < 3; i++) {
        int height = 0;
        if (tryParse(config["height"], height))
            _m_dashboardLayout.m_skillButtonsSize[i].setHeight(height);
        int width = 0;
        if (i < configWidth.size() && tryParse(configWidth[i], width))
            _m_dashboardLayout.m_skillButtonsSize[i].setWidth(width);
        if (i < configTextArea.size())
            tryParse(configTextArea[i], _m_dashboardLayout.m_skillTextArea[i]);
        if (i < configTextAreaDown.size())
            tryParse(configTextAreaDown[i], _m_dashboardLayout.m_skillTextAreaDown[i]);
        if (i < configTextFont.size())
            _m_dashboardLayout.m_skillTextFonts[i].tryParse(configTextFont[i]);
    }
    for (int i = 0; i < QSanInvokeSkillButton::S_NUM_SKILL_TYPES; i++) {
        QString key;
        switch ((QSanInvokeSkillButton::SkillType)i) {
        case QSanInvokeSkillButton::S_SKILL_AWAKEN: key = "awakenFontColor"; break;
        case QSanInvokeSkillButton::S_SKILL_COMPULSORY: key = "compulsoryFontColor"; break;
        case QSanInvokeSkillButton::S_SKILL_FREQUENT: key = "frequentFontColor"; break;
        case QSanInvokeSkillButton::S_SKILL_ONEOFF_SPELL: key = "oneoffFontColor"; break;
        case QSanInvokeSkillButton::S_SKILL_PROACTIVE: key = "proactiveFontColor"; break;
        case QSanInvokeSkillButton::S_SKILL_ATTACHEDLORD: key = "attachedlordFontColor"; break;
        default:
            Q_ASSERT(false);
            break;
        }

        JsonArray subconfig = config[key].value<JsonArray>();
        for (int j = 0; j < 4 && j < subconfig.size(); j++) {
            int index = i * 4 + j;
            JsonArray config = subconfig[j].value<JsonArray>();
            if (config.size() < 2)
                continue;
            tryParse(config[0], _m_dashboardLayout.m_skillTextColors[index]);
            tryParse(config[1], _m_dashboardLayout.m_skillTextShadowColors[index]);
        }
    }

    return true;
}

bool QSanSkinScheme::load(const QVariant &configs)
{
    JsonObject config = configs.value<JsonObject>();
    if (config.isEmpty()) return false;
    QString layoutFile = config["roomLayoutConfigFile"].toString();
    QString imageFile = config["roomImageConfigFile"].toString();
    QString audioFile = config["roomAudioConfigFile"].toString();
    QString animFile = config["roomAnimationConfigFile"].toString();
    return _m_roomSkin.load(layoutFile, imageFile, audioFile, animFile);
}

const QSanRoomSkin &QSanSkinScheme::getRoomSkin() const
{
    return _m_roomSkin;
}

QSanSkinFactory &QSanSkinFactory::getInstance()
{
    if (_sm_singleton == NULL) {
#ifdef Q_OS_WIN
        _sm_singleton = new QSanSkinFactory("skins/skinList.json");
#else
        _sm_singleton = new QSanSkinFactory("skins/skinListAlt.json");
#endif
    }
    return *_sm_singleton;
}

void QSanSkinFactory::destroyInstance()
{
    if (_sm_singleton) {
        delete _sm_singleton;
        _sm_singleton = NULL;
    }
}

const QSanSkinScheme &QSanSkinFactory::getCurrentSkinScheme()
{
    return this->_sm_currentSkin;
}

bool QSanSkinFactory::switchSkin(QString skinName)
{
    if (skinName == _m_skinName) return false;
    bool success = false;
    if (_m_skinName != S_DEFAULT_SKIN_NAME) {
        success = _sm_currentSkin.load(_m_skinList[S_DEFAULT_SKIN_NAME.toLatin1().constData()]);
        if (!success) qWarning("Cannot load default skin!");
    }
    if (skinName != S_DEFAULT_SKIN_NAME)
        success = _sm_currentSkin.load(_m_skinList[skinName.toLatin1().constData()]);
    if (!success)
        qWarning("Loading skin %s failed", skinName.toLatin1().constData());
    _m_skinName = skinName;
    return success;
}

QSanSkinFactory::QSanSkinFactory(const char *fileName)
{
    S_DEFAULT_SKIN_NAME = "fulldefault";
    S_COMPACT_SKIN_NAME = "fullcompact";

    JsonDocument doc = JsonDocument::fromFilePath(fileName);
    _m_skinList = doc.object();
    _m_skinName = "";
    switchSkin(S_DEFAULT_SKIN_NAME);
}

const QString &QSanSkinFactory::getCurrentSkinName() const
{
    return _m_skinName;
}

