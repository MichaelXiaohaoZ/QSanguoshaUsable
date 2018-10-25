#include "yearboss.h"
#include "settings.h"
#include "skill.h"
#include "standard.h"
#include "client.h"
#include "clientplayer.h"
#include "engine.h"
#include "maneuvering.h"
#include "roomscene.h"
#include "special3v3.h"

class YearRuishou : public TriggerSkill
{
public:
    YearRuishou() : TriggerSkill("yearruishou")
    {
        events << DamageForseen;
        frequency = Compulsory;
    }

    virtual bool triggerable(const ServerPlayer *target, Room *room) const
    {
        return target != NULL;
    }

    virtual bool trigger(TriggerEvent triggerEvent, Room *room, ServerPlayer *player, QVariant &data) const
    {
        DamageStruct damage = data.value<DamageStruct>();
        bool whether = false;
        if (damage.from->hasSkill(objectName()))
        {
            foreach (ServerPlayer *p, room->getOtherPlayers(damage.from, false))
                if (p->getKingdom() == damage.from->getKingdom())
                    whether = true;
        }
        else if (damage.to->hasSkill(objectName()))
        {
            foreach (ServerPlayer *p, room->getOtherPlayers(damage.to, false))
                if (p->getKingdom() == damage.to->getKingdom())
                    whether = true;
        }
        if (whether && damage.to->getKingdom() != damage.from->getKingdom())
        {
            if (damage.from->hasSkill(objectName()))
            {
                room->broadcastSkillInvoke(objectName(), damage.from);
                LogMessage rslog;
                rslog.type = "#ruishoulog";
                rslog.from = damage.from;
                rslog.arg = QString::number(damage.damage);
                room->sendLog(rslog);
            }
            if (damage.to->hasSkill(objectName()))
            {
                room->broadcastSkillInvoke(objectName(), damage.to);
                LogMessage rslog;
                rslog.type = "#ruishoulog";
                rslog.from = damage.to;
                rslog.arg = QString::number(damage.damage);
                room->sendLog(rslog);
            }
            return true;
        }
        return false;
    }
};

YearZishuCard::YearZishuCard()
{

}

bool YearZishuCard::targetFixed() const
{
    return true;
}

void YearZishuCard::use(Room *room, ServerPlayer *source, QList<ServerPlayer *> &targets) const
{
    while (true)
    {
        QList<ServerPlayer *> can_targets;
        foreach (ServerPlayer *target, room->getOtherPlayers(source, false))
            if (canUseFromOther(room, source, target))
                can_targets << target;
        if (!can_targets.isEmpty())
        {
            ServerPlayer *targetc =
                    room->askForPlayerChosen(source, can_targets, "yearzishu", QString(), true, true);
            if (targetc)
            {
                int card_id = room->askForCardChosen(source, targetc, "h", "yearzishu");
                CardMoveReason reason(CardMoveReason::S_REASON_EXTRACTION, source->objectName());
                room->obtainCard(source, Sanguosha->getCard(card_id), reason, false);
                room->setPlayerFlag(source, "YearZishuUsed");
                continue;
            }
        }
        break;
    }

}

bool YearZishuCard::canUseFromOther(Room *room, ServerPlayer *source, ServerPlayer *target) const
{
    return target != NULL && source->getHandcardNum() < target->getHandcardNum() && !target->isKongcheng();
}

class YearZishu : public ZeroCardViewAsSkill
{
public:
    YearZishu() : ZeroCardViewAsSkill("yearzishu")
    {

    }

    virtual bool isEnabledAtPlay(const Player *player) const
    {
        if (player->hasFlag("YearZishuUsed"))
            return false;
        foreach (const Player *p, player->getSiblings())
            if (p->getHandcardNum() > player->getHandcardNum() && !p->isKongcheng())
                return true;
        return false;
    }

    virtual bool isEnabledAtResponse(const Player *, const QString &) const
    {
        return false;
    }

    virtual bool isEnabledAtNullification(const ServerPlayer *) const
    {
        return false;
    }

    virtual const Card *viewAs() const
    {
        return new YearZishuCard;
    }
};

class YearChouniu : public TriggerSkill
{
public:
    YearChouniu() : TriggerSkill("yearchouniu")
    {
        events << TurnStart << EventPhaseStart;
        frequency = Compulsory;
    }

    virtual bool trigger(TriggerEvent triggerEvent, Room *room, ServerPlayer *player, QVariant &data) const
    {
        if (triggerEvent == TurnStart)
        {
            if (room->getTag("FirstRound").toBool())
                room->loseHp(player, player->getHp() - 1);
        }
        else
        {
            bool min = true;
            foreach (ServerPlayer *p, room->getAlivePlayers())
                if (p->getHp() > player->getHp())
                {
                    min = false;
                    break;
                }
            if (min)
                room->recover(player, RecoverStruct(player));
        }
        return false;
    }
};

YearYinhuCard::YearYinhuCard()
{

}

void YearYinhuCard::use(Room *room, ServerPlayer *source, QList<ServerPlayer *> &targets) const
{
    room->setPlayerFlag(source, "YinhuUsed" + Sanguosha->getCard(getEffectiveId())->getType());
    foreach (ServerPlayer *target, targets)
        room->damage(DamageStruct("yearyinhu", source, target));
}

class YearYinhuVS : public OneCardViewAsSkill
{
public:
    YearYinhuVS() : OneCardViewAsSkill("yearyinhu")
    {
        filter_pattern = ".|.|.|hand";
    }

    virtual bool isEnabledAtPlay(const Player *player) const
    {
        return !player->hasFlag("YinhuDying");
    }

    virtual bool isEnabledAtResponse(const Player *player, const QString &pattern) const
    {
        return false;
    }

    virtual bool isEnabledAtNullification(const ServerPlayer *player) const
    {
        return false;
    }

    virtual bool viewFilter(const Card *to_select) const
    {
        return !Self->isJilei(to_select) && !Self->hasFlag("YinhuUsed"+to_select->getType());
    }

    virtual const Card *viewAs(const Card *originalCard) const
    {
        YearYinhuCard *card = new YearYinhuCard;
        card->addSubcard(originalCard);
        return card;
    }
};

class YearYinhu : public TriggerSkill
{
public:
    YearYinhu() : TriggerSkill("yearyinhu")
    {
        events << EnterDying;
        view_as_skill = new YearYinhuVS;
    }

    bool triggerable(const ServerPlayer *target) const
    {
        return target != NULL;
    }

    bool trigger(TriggerEvent, Room *room, ServerPlayer *, QVariant &data) const
    {
        DyingStruct dying = data.value<DyingStruct>();
        if (dying.damage && dying.damage->getReason() == "yearyinhu")
        {
            ServerPlayer *from = dying.damage->from;
            if (from && from->isAlive()) {
                room->setPlayerFlag(from, "YinhuDying");
            }
        }
        return false;
    }
};

class YearMaotu : public ProhibitSkill
{
public:
    YearMaotu() : ProhibitSkill("yearmaotu")
    {

    }

    virtual bool isProhibited(const Player *from, const Player *to, const Card *card, const QList<const Player *> &others) const
    {
        if (!to->hasSkill(objectName())) return false;

        bool samek = false;
        foreach (const Player *other, to->getSiblings())
            if (other->getKingdom() == to->getKingdom())
            {
                samek = true;
                break;
            }
        if (samek && from->getKingdom() != to->getKingdom() &&
                from->getHp() >= to->getHp() && to->getRole() != from->getRole())
            return true;

        return false;
    }
};

YearChenlongCard::YearChenlongCard()
{

}

void YearChenlongCard::extraCost(Room *room, const CardUseStruct &card_use) const
{
    room->removePlayerMark(card_use.from, "@yearchenlong");
    room->setPlayerMark(card_use.from, "chenlongusing", 1);
    room->loseHp(card_use.from, user_string.toInt());
    room->setPlayerMark(card_use.from, "chenlongusing", 0);
}

void YearChenlongCard::use(Room *room, ServerPlayer *source, QList<ServerPlayer *> &targets) const
{
    //room->setPlayerMark(source, "#qimou", user_string.toInt())
    foreach (ServerPlayer *target, targets)
        room->damage(DamageStruct("yearchenlong", source, target, user_string.toInt()));
}

class YearChenlongVS : public ZeroCardViewAsSkill
{
public:
    YearChenlongVS() : ZeroCardViewAsSkill("yearchenlong")
    {

    }

    virtual bool isEnabledAtPlay(const Player *player) const
    {
        return player->getMark("@yearchenlong") > 0 && player->getHp() > 0;
    }

    virtual bool isEnabledAtResponse(const Player *, const QString &) const
    {
        return false;
    }

    virtual bool isEnabledAtNullification(const ServerPlayer *) const
    {
        return false;
    }

    virtual const Card *viewAs() const
    {
        QString user_string = Self->tag["yearchenlong"].toString();
        if (user_string.isEmpty()) return NULL;
        YearChenlongCard *skill_card = new YearChenlongCard;
        skill_card->setUserString(user_string);
        skill_card->setSkillName("yearchenlong");
        return skill_card;
    }
};

class YearChenlong : public TriggerSkill
{
public:
    YearChenlong() : TriggerSkill("yearchenlong")
    {
        events << EnterDying;
        frequency = Limited;
        limit_mark = "@yearchenlong";
        view_as_skill = new YearChenlongVS;
    }

    QString getSelectBox() const
    {
        QStringList hp_num;
        for (int i = 1; i <= Self->getHp() && i <= 5; i++)
            hp_num << QString::number(i);
        return hp_num.join("+");
    }

    virtual bool trigger(TriggerEvent triggerEvent, Room *room, ServerPlayer *player, QVariant &data) const
    {
        if (player->getMark("chenlongusing"))
        {
            room->recover(player, RecoverStruct(player, NULL, 1 - player->getHp()));
            room->loseMaxHp(player, player->getMaxHp() - 1);
            room->setPlayerMark(player, "chenlongusing", 0);
        }
        return false;
    }
};

class YearChuanchengChenlong : public TriggerSkill
{
public:
    YearChuanchengChenlong() : TriggerSkill("yearchuanchengchenlong")
    {
        events << Death;
        frequency = Compulsory;
    }

    virtual bool triggerable(const ServerPlayer *target) const
    {
        return target != NULL && target->hasSkill(this);
    }

    virtual bool trigger(TriggerEvent triggerEvent, Room *room, ServerPlayer *player, QVariant &data) const
    {
        DeathStruct death = data.value<DeathStruct>();
        if (player->hasSkill(objectName()) && death.who == player && death.damage && death.damage->from)
        {
            if (death.damage->from->hasSkill("yearchenlong") && death.damage->from->getMark("chuanchengedchenlong"))
            {
                room->detachSkillFromPlayer(death.damage->from, "yearchenlong");
                room->setPlayerMark(death.damage->from, "chuanchengedchenlong", 0);
            }
            if (death.damage->from->hasSkill("yearweiyang") && death.damage->from->getMark("chuanchengedweiyang"))
            {
                room->detachSkillFromPlayer(death.damage->from, "yearweiyang");
                room->setPlayerMark(death.damage->from, "chuanchengedweiyang", 0);
            }
            if (death.damage->from->hasSkill("yearshenhou") && death.damage->from->getMark("chuanchengedshenhou"))
            {
                room->detachSkillFromPlayer(death.damage->from, "yearshenhou");
                room->setPlayerMark(death.damage->from, "chuanchengedshenhou", 0);
            }
            if (death.damage->from->hasSkill("yearxugou") && death.damage->from->getMark("chuanchengedxugou"))
            {
                room->detachSkillFromPlayer(death.damage->from, "yearxugou");
                room->setPlayerMark(death.damage->from, "chuanchengedxugou", 0);
            }
            room->acquireSkill(death.damage->from, "yearchenlong");
            room->addPlayerMark(death.damage->from, "chuanchengedchenlong");
        }
        return false;
    }
};

class YearSishe : public TriggerSkill
{
public:
    YearSishe() : TriggerSkill("yearsishe")
    {
        events << Damaged;
        frequency = Frequent;
    }

    virtual bool trigger(TriggerEvent triggerEvent, Room *room, ServerPlayer *player, QVariant &data) const
    {
        DamageStruct damage = data.value<DamageStruct>();
        if (damage.from && damage.from != player && room->askForSkillInvoke(player, objectName()))
        {
            room->broadcastSkillInvoke(objectName(), player);
            room->damage(DamageStruct(objectName(), player, damage.from, damage.damage));
        }
        return false;
    }
};

class YearWuma : public TriggerSkill
{
public:
    YearWuma() : TriggerSkill("yearwuma")
    {
        events << EventPhaseSkipping << TargetConfirmed;
        frequency = Compulsory;
    }

    virtual bool trigger(TriggerEvent triggerEvent, Room *room, ServerPlayer *player, QVariant &data) const
    {
        if (triggerEvent == EventPhaseSkipping)
        {
            room->broadcastSkillInvoke(objectName(), player);
            return true;
        }
        else
        {
            CardUseStruct use = data.value<CardUseStruct>();
            if (use.card->isKindOf("TrickCard") && use.to.contains(player))
            {
                room->broadcastSkillInvoke(objectName(), player);
                player->drawCards(1);
            }
        }
        return false;
    }

    virtual int getPriority(TriggerEvent triggerEvent) const
    {
        if (triggerEvent == EventPhaseSkipping) return 6;
        return 3;
    }
};

YearWeiyangCard::YearWeiyangCard()
{

}

bool YearWeiyangCard::targetFixed() const
{
    return true;
}

void YearWeiyangCard::use(Room *room, ServerPlayer *source, QList<ServerPlayer *> &) const
{
    QList<ServerPlayer *> cantargets;
    foreach (ServerPlayer *p, room->getAlivePlayers())
        if (p->isWounded()) cantargets << p;
    QList<ServerPlayer *> ntargets =
            room->askForPlayersChosen(source, cantargets, "yearweiyang", 1, subcards.length());
    foreach (ServerPlayer *target, ntargets)
        room->recover(target, RecoverStruct(source, this, 1), true);
    room->setPlayerFlag(source, "YearWeiyangUsed");
}

class YearWeiyang : public ViewAsSkill
{
public:
    YearWeiyang() : ViewAsSkill("yearweiyang")
    {

    }

    virtual bool viewFilter(const QList<const Card *> &selected, const Card *to_select) const
    {
        foreach (const Card *selectedcard, selected)
            if (to_select->getType() == selectedcard->getType())
                return false;
        return !Self->isJilei(to_select);
    }

    virtual const Card *viewAs(const QList<const Card *> &cards) const
    {
        if (cards.isEmpty())
            return NULL;
        YearWeiyangCard *weiyangc = new YearWeiyangCard;
        weiyangc->addSubcards(cards);
        weiyangc->setSkillName(objectName());
        return weiyangc;
    }

    virtual bool isEnabledAtPlay(const Player *player) const
    {
        if (player->hasFlag("YearWeiyangUsed")) return false;
        if (player->isWounded()) return true;
        foreach (const Player *p, player->getSiblings())
            if (p->isWounded()) return true;
        return false;
    }

    virtual bool isEnabledAtResponse(const Player *, const QString &) const
    {
        return false;
    }

    virtual bool isEnabledAtNullification(const ServerPlayer *) const
    {
        return false;
    }
};

class YearChuanchengWeiyang : public TriggerSkill
{
public:
    YearChuanchengWeiyang() : TriggerSkill("yearchuanchengweiyang")
    {
        events << Death;
        frequency = Compulsory;
    }

    virtual bool triggerable(const ServerPlayer *target) const
    {
        return target != NULL && target->hasSkill(this);
    }

    virtual bool trigger(TriggerEvent triggerEvent, Room *room, ServerPlayer *player, QVariant &data) const
    {
        DeathStruct death = data.value<DeathStruct>();
        if (player->hasSkill(objectName()) && death.who == player && death.damage && death.damage->from)
        {
            if (death.damage->from->hasSkill("yearchenlong") && death.damage->from->getMark("chuanchengedchenlong"))
            {
                room->detachSkillFromPlayer(death.damage->from, "yearchenlong");
                room->setPlayerMark(death.damage->from, "chuanchengedchenlong", 0);
            }
            if (death.damage->from->hasSkill("yearweiyang") && death.damage->from->getMark("chuanchengedweiyang"))
            {
                room->detachSkillFromPlayer(death.damage->from, "yearweiyang");
                room->setPlayerMark(death.damage->from, "chuanchengedweiyang", 0);
            }
            if (death.damage->from->hasSkill("yearshenhou") && death.damage->from->getMark("chuanchengedshenhou"))
            {
                room->detachSkillFromPlayer(death.damage->from, "yearshenhou");
                room->setPlayerMark(death.damage->from, "chuanchengedshenhou", 0);
            }
            if (death.damage->from->hasSkill("yearxugou") && death.damage->from->getMark("chuanchengedxugou"))
            {
                room->detachSkillFromPlayer(death.damage->from, "yearxugou");
                room->setPlayerMark(death.damage->from, "chuanchengedxugou", 0);
            }
            room->acquireSkill(death.damage->from, "yearweiyang");
            room->addPlayerMark(death.damage->from, "chuanchengedweiyang");
        }
        return false;
    }
};

class YearShenhou : public TriggerSkill
{
public:
    YearShenhou() : TriggerSkill("yearshenhou")
    {
        events << TargetConfirmed;
        frequency = Frequent;
    }

    virtual bool trigger(TriggerEvent triggerEvent, Room *room, ServerPlayer *player, QVariant &data) const
    {
        CardUseStruct use = data.value<CardUseStruct>();
        if (use.card && use.card->isKindOf("Slash"))
            if (room->askForSkillInvoke(player, objectName()))
            {
                room->broadcastSkillInvoke(objectName(), player);
                JudgeStruct judge;
                judge.pattern = ".|red";
                judge.good = true;
                judge.reason = objectName();
                judge.who = player;
                room->judge(judge);
                if (judge.isGood())
                {
                    LogMessage log;
                    log.type = "#shenhoueffectlog";
                    log.from = player;
                    room->sendLog(log);
                    return true;
                }
            }
        return false;
    }
};

class YearChuanchengShenhou : public TriggerSkill
{
public:
    YearChuanchengShenhou() : TriggerSkill("yearchuanchengshouhou")
    {
        events << Death;
        frequency = Compulsory;
    }

    virtual bool triggerable(const ServerPlayer *target) const
    {
        return target != NULL && target->hasSkill(this);
    }

    virtual bool trigger(TriggerEvent triggerEvent, Room *room, ServerPlayer *player, QVariant &data) const
    {
        DeathStruct death = data.value<DeathStruct>();
        if (player->hasSkill(objectName()) && death.who == player && death.damage && death.damage->from)
        {
            if (death.damage->from->hasSkill("yearchenlong") && death.damage->from->getMark("chuanchengedchenlong"))
            {
                room->detachSkillFromPlayer(death.damage->from, "yearchenlong");
                room->setPlayerMark(death.damage->from, "chuanchengedchenlong", 0);
            }
            if (death.damage->from->hasSkill("yearweiyang") && death.damage->from->getMark("chuanchengedweiyang"))
            {
                room->detachSkillFromPlayer(death.damage->from, "yearweiyang");
                room->setPlayerMark(death.damage->from, "chuanchengedweiyang", 0);
            }
            if (death.damage->from->hasSkill("yearshenhou") && death.damage->from->getMark("chuanchengedshenhou"))
            {
                room->detachSkillFromPlayer(death.damage->from, "yearshenhou");
                room->setPlayerMark(death.damage->from, "chuanchengedshenhou", 0);
            }
            if (death.damage->from->hasSkill("yearxugou") && death.damage->from->getMark("chuanchengedxugou"))
            {
                room->detachSkillFromPlayer(death.damage->from, "yearxugou");
                room->setPlayerMark(death.damage->from, "chuanchengedxugou", 0);
            }
            room->acquireSkill(death.damage->from, "yearshenhou");
            room->addPlayerMark(death.damage->from, "chuanchengedshenhou");
        }
        return false;
    }
};

class YearYouji : public DrawCardsSkill
{
public:
    YearYouji() : DrawCardsSkill("yearyouji")
    {
        frequency = Compulsory;
    }

    virtual int getDrawNum(ServerPlayer *player, int n) const
    {
        Room *room = player->getRoom();
        return n + room->getTurn();
    }
};

class YearXugou : public TriggerSkill
{
public:
    YearXugou() : TriggerSkill("yearxugou")
    {
        events << SlashEffected << DamageCaused;
        frequency = Compulsory;
    }

    virtual bool triggerable(const ServerPlayer *target) const
    {
        return target != NULL && TriggerSkill::triggerable(target);
    }

    virtual bool trigger(TriggerEvent triggerEvent, Room *room, ServerPlayer *player, QVariant &data) const
    {
        if (triggerEvent == SlashEffected)
        {
            SlashEffectStruct effect = data.value<SlashEffectStruct>();
            if (effect.slash->isRed()) {
                player->broadcastSkillInvoke(objectName());
                room->notifySkillInvoked(player, objectName());

                LogMessage log;
                log.type = "#SkillNullify";
                log.from = player;
                log.arg = objectName();
                log.arg2 = effect.slash->objectName();
                room->sendLog(log);

                return true;
            }
        }
        else
        {
            if (!player->isAlive())
                return false;
            DamageStruct damage = data.value<DamageStruct>();
            if (damage.chain || damage.transfer || !damage.by_user) return false;
            const Card *reason = damage.card;
            if (reason && reason->isKindOf("Slash") && reason->isRed())
            {
                LogMessage log;
                log.type = "#YearXugouBuff";
                log.from = player;
                log.to << damage.to;
                room->sendLog(log);
                damage.damage++;
                data = QVariant::fromValue(damage);
            }
        }
        return false;
    }
};

class YearXugouTargetMod : public TargetModSkill
{
public:
    YearXugouTargetMod() : TargetModSkill("#yearxugou-target")
    {
    }

    virtual int getDistanceLimit(const Player *from, const Card *card, const Player *) const
    {
        if (from->hasSkill("yearxugou") && card->isRed() && card->isKindOf("Slash"))
            return 1000;
        else
            return 0;
    }
};

class YearChuanchengXugou : public TriggerSkill
{
public:
    YearChuanchengXugou() : TriggerSkill("yearchuanchengxugou")
    {
        events << Death;
        frequency = Compulsory;
    }

    virtual bool triggerable(const ServerPlayer *target) const
    {
        return target != NULL && target->hasSkill(this);
    }

    virtual bool trigger(TriggerEvent triggerEvent, Room *room, ServerPlayer *player, QVariant &data) const
    {
        DeathStruct death = data.value<DeathStruct>();
        if (player->hasSkill(objectName()) && death.who == player && death.damage && death.damage->from)
        {
            if (death.damage->from->hasSkill("yearchenlong") && death.damage->from->getMark("chuanchengedchenlong"))
            {
                room->detachSkillFromPlayer(death.damage->from, "yearchenlong");
                room->setPlayerMark(death.damage->from, "chuanchengedchenlong", 0);
            }
            if (death.damage->from->hasSkill("yearweiyang") && death.damage->from->getMark("chuanchengedweiyang"))
            {
                room->detachSkillFromPlayer(death.damage->from, "yearweiyang");
                room->setPlayerMark(death.damage->from, "chuanchengedweiyang", 0);
            }
            if (death.damage->from->hasSkill("yearshenhou") && death.damage->from->getMark("chuanchengedshenhou"))
            {
                room->detachSkillFromPlayer(death.damage->from, "yearshenhou");
                room->setPlayerMark(death.damage->from, "chuanchengedshenhou", 0);
            }
            if (death.damage->from->hasSkill("yearxugou") && death.damage->from->getMark("chuanchengedxugou"))
            {
                room->detachSkillFromPlayer(death.damage->from, "yearxugou");
                room->setPlayerMark(death.damage->from, "chuanchengedxugou", 0);
            }
            room->acquireSkill(death.damage->from, "yearxugou");
            room->addPlayerMark(death.damage->from, "chuanchengedxugou");
        }
        return false;
    }
};

class YearHaizhu : public TriggerSkill
{
public:
    YearHaizhu() : TriggerSkill("yearhaizhu")
    {
        events << CardsMoveOneTime << EventPhaseStart;
        frequency = Compulsory;
    }

    virtual bool trigger(TriggerEvent triggerEvent, Room *room, ServerPlayer *caozhi, QVariant &data) const
    {
        if (triggerEvent == CardsMoveOneTime)
        {
            CardsMoveOneTimeStruct move = data.value<CardsMoveOneTimeStruct>();
            if (move.from == caozhi || move.from == NULL)
                return false;
            if (move.to_place == Player::DiscardPile
                && ((move.reason.m_reason & CardMoveReason::S_MASK_BASIC_REASON) == CardMoveReason::S_REASON_DISCARD
                || move.reason.m_reason == CardMoveReason::S_REASON_JUDGEDONE))
            {
                QList<int> card_ids;
                int i = 0;
                foreach (int card_id, move.card_ids)
                {
                    if ((Sanguosha->getCard(card_id)->getSuit() == Card::Club
                        && ((move.reason.m_reason != CardMoveReason::S_REASON_JUDGEDONE
                        && (move.from_places[i] == Player::PlaceHand || move.from_places[i] == Player::PlaceEquip))))
                        && move.active_ids.contains(card_id) && room->getDiscardPile().contains(card_id))
                        card_ids << card_id;
                    {
                        i++;
                    }
                }
                if (card_ids.isEmpty())
                    return false;
                else
                {
                    foreach (int id, card_ids) {
                        move.active_ids.removeOne(id);
                    }
                    data = QVariant::fromValue(move);
                    DummyCard *dummy = new DummyCard(card_ids);
                    room->obtainCard(caozhi, dummy);
                    delete dummy;
                }
            }
        }
        else
            if (caozhi->getPhase() == Player::Start)
            {
                foreach (ServerPlayer *p, room->getOtherPlayers(caozhi))
                    if (p->getHandcardNum() > caozhi->getHandcardNum())
                        return false;
                room->loseHp(caozhi);
            }
        return false;
    }
};

class YearJiyuan : public TriggerSkill
{
public:
    YearJiyuan() : TriggerSkill("yearjiyuan")
    {
        events << EventPhaseStart;
        frequency = Compulsory;
    }

    virtual bool trigger(TriggerEvent, Room *room, ServerPlayer *player, QVariant &data) const
    {
        if (player->getPhase() == Player::Finish)
            room->drawCards(player, player->getMaxHp() / 2 + player->getMaxHp()%2, objectName());
        return false;
    }
};

class YearSuizhongEasy : public TriggerSkill
{
public:
    YearSuizhongEasy() : TriggerSkill("yearsuizhongeasy")
    {
        events << AskForPeaches;
        frequency = Limited;
        limit_mark = "@suizhong";
    }

    virtual bool triggerable(const ServerPlayer *target) const
    {
        return TriggerSkill::triggerable(target) && target->getMark("@suizhong") > 0;
    }

    virtual bool trigger(TriggerEvent, Room *room, ServerPlayer *pangtong, QVariant &data) const
    {
        DyingStruct dying_data = data.value<DyingStruct>();
        if (dying_data.who != pangtong)
            return false;

        if (pangtong->askForSkillInvoke(this, data))
        {
            pangtong->broadcastSkillInvoke(objectName());
            room->removePlayerMark(pangtong, "@suizhong");
            room->recover(pangtong, RecoverStruct(pangtong, NULL, 1 - pangtong->getHp()));
            if (pangtong->getPhase() == Player::NotActive)
                throw TurnBroken;
        }

        return false;
    }
};

class YearSuizhong : public TriggerSkill
{
public:
    YearSuizhong() : TriggerSkill("yearsuizhong")
    {
        events << AskForPeaches;
        frequency = Limited;
        limit_mark = "@suizhong";
    }

    virtual bool triggerable(const ServerPlayer *target) const
    {
        return TriggerSkill::triggerable(target) && target->getMark("@suizhong") > 0;
    }

    virtual bool trigger(TriggerEvent, Room *room, ServerPlayer *pangtong, QVariant &data) const
    {
        DyingStruct dying_data = data.value<DyingStruct>();
        if (dying_data.who != pangtong)
            return false;

        if (pangtong->askForSkillInvoke(this, data))
        {
            pangtong->broadcastSkillInvoke(objectName());
            room->removePlayerMark(pangtong, "@suizhong");
            room->recover(pangtong, RecoverStruct(pangtong, NULL, 1 - pangtong->getHp()));

            foreach (ServerPlayer *player, room->getOtherPlayers(pangtong))
                player->throwAllHandCardsAndEquips();

            if (pangtong->getPhase() == Player::NotActive)
                throw TurnBroken;
        }

        return false;
    }
};

class YearCuikuEasy : public TriggerSkill
{
public:
    YearCuikuEasy() : TriggerSkill("yearcuikueasy")
    {
        events << RoundStart;
        frequency = Compulsory;
    }

    virtual bool trigger(TriggerEvent triggerEvent, Room *room, ServerPlayer *player, QVariant &data) const
    {
        if (room->getTurn()%6 == 1 && player->getSeat() == 1)
        {
            ServerPlayer *target =
                    room->askForPlayerChosen(player, room->getOtherPlayers(player), objectName(), QString(), true);
            if (target)
                room->damage(DamageStruct(objectName(), player, target, 2));
        }
    }
};

class YearCuiku : public TriggerSkill
{
public:
    YearCuiku() : TriggerSkill("yearcuiku")
    {
        events << RoundStart;
        frequency = Compulsory;
    }

    virtual bool trigger(TriggerEvent triggerEvent, Room *room, ServerPlayer *player, QVariant &data) const
    {
        if (room->getTurn()%6 == 1 && player->getSeat() == 1)
        {
            QList<ServerPlayer *> targets =
                    room->askForPlayersChosen(player, room->getOtherPlayers(player), objectName(), 0, 2, QString(), true);
            if (!targets.isEmpty())
                foreach (ServerPlayer *target, targets)
                    room->damage(DamageStruct(objectName(), player, target, 2));
        }
    }
};

class YearCuikuDifficult : public TriggerSkill
{
public:
    YearCuikuDifficult() : TriggerSkill("yearcuikudifficult")
    {
        events << RoundStart;
        frequency = Compulsory;
    }

    virtual bool trigger(TriggerEvent triggerEvent, Room *room, ServerPlayer *player, QVariant &data) const
    {
        if (room->getTurn()%6 == 1 && player->getSeat() == 1)
        {
            foreach (ServerPlayer *target, room->getOtherPlayers(player))
            {
                room->damage(DamageStruct(objectName(), player, target, target->getMaxHp() / 2));
                if (target->getMaxHp()%2)
                    room->drawCards(player, 1, objectName());
            }
        }
    }
};

class YearNianyi : public TriggerSkill
{
public:
    YearNianyi() : TriggerSkill("yearnianyi")
    {
        events << EventPhaseStart;
        frequency = Compulsory;
    }

    virtual bool trigger(TriggerEvent triggerEvent, Room *room, ServerPlayer *player, QVariant &data) const
    {
        if (player->getPhase() == Player::Start)
        {
            QList<const Card *> tricks = player->getJudgingArea();
            if (!tricks.isEmpty())
            {
                int index = qrand()%tricks.length();
                CardMoveReason reason(CardMoveReason::S_REASON_THROW, player->objectName(), objectName(), QString());
                room->throwCard(tricks.at(index), reason, player, NULL);
            }
        }
    }
};

class YearNianyiDifficult : public TriggerSkill
{
public:
    YearNianyiDifficult() : TriggerSkill("yearnianyidifficult")
    {
        events << EventPhaseStart << CardsMoveOneTime;
        frequency = Compulsory;
    }

    virtual bool trigger(TriggerEvent triggerEvent, Room *room, ServerPlayer *player, QVariant &data) const
    {
        if (triggerEvent == EventPhaseStart)
        {
            if (player->getPhase() == Player::Start)
            {
                QList<const Card *> tricks = player->getJudgingArea();
                if (!tricks.isEmpty())
                {
                    int index = qrand()%tricks.length();
                    CardMoveReason reason(CardMoveReason::S_REASON_THROW, player->objectName(), objectName(), QString());
                    room->throwCard(tricks.at(index), reason, player, NULL);
                }
            }
        }
        else if (triggerEvent == CardsMoveOneTime)
        {
            if (player->getPhase() == Player::NotActive)
            {
                CardsMoveOneTimeStruct move = data.value<CardsMoveOneTimeStruct>();
                if (move.from == player && (move.from_places.contains(Player::PlaceHand)
                                            || move.from_places.contains(Player::PlaceEquip))
                    && !(move.to == player && (move.to_place == Player::PlaceHand
                                               || move.to_place == Player::PlaceEquip)))
                {
                    room->addPlayerMark(player, "#nianyi", 1);
                }
            }
        }
    }
};

class YearNianyiDifficultEffect : public PhaseChangeSkill
{
public:
    YearNianyiDifficultEffect() : PhaseChangeSkill("#yearnianyidifficult-effect")
    {

    }

    virtual bool triggerable(const ServerPlayer *target) const
    {
        return target != NULL;
    }

    virtual bool onPhaseChange(ServerPlayer *player) const
    {
        Room *room = player->getRoom();
        if (player->getPhase() != Player::Finish) return false;
        foreach (ServerPlayer *boss, room->getAllPlayers())
        {
            if (boss->hasSkill(objectName()))
                if (boss->getMark("#nianyi"))
                {
                    if (boss->getMark("#nianyi") > 2 && boss->getPhase() == Player::NotActive)
                        foreach (ServerPlayer *p, room->getOtherPlayers(boss))
                            room->damage(DamageStruct("yearnianyidifficult", boss, p));
                    room->setPlayerMark(boss, "#nianyi", 0);
                }
        }
    }
};

class YearNianyiTargetMod : public TargetModSkill
{
public:
    YearNianyiTargetMod() : TargetModSkill("#yearnianyi-target")
    {
        pattern = ".";
    }

    virtual int getDistanceLimit(const Player *from, const Card *card, const Player *to) const
    {
        if ((from->hasSkill("yearnianyi") || from->hasSkill("yearnianyidifficult")) && to)
            return 10000;
        return 0;
    }
};

YearBossPackage::YearBossPackage()
    : Package("YearBoss")
{
    General *zishu = new General(this, "bosszishu", "god", 3, true, true);
    zishu->addSkill(new YearZishu);
    zishu->addSkill(new YearRuishou);

    General *chouniu = new General(this, "bosschouniu", "god", 5, true, true);
    chouniu->addSkill(new YearChouniu);
    chouniu->addSkill("yearruishou");

    General *yinhu = new General(this, "bossyinhu", "god", 4, true, true);
    yinhu->addSkill(new YearYinhu);
    yinhu->addSkill("yearruishou");

    General *maotu = new General(this, "bossmaotu", "god", 3, false, true);
    maotu->addSkill(new YearMaotu);
    maotu->addSkill("yearruishou");

    General *chenlong = new General(this, "bosschenlong", "god", 4, true, true);
    chenlong->addSkill(new YearChenlong);
    chenlong->addSkill(new YearChuanchengChenlong);
    chenlong->addSkill("yearruishou");

    General *sishe = new General(this, "bosssishe", "god", 3, false, true);
    sishe->addSkill(new YearSishe);
    sishe->addSkill("yearruishou");

    General *wuma = new General(this, "bosswuma", "god", 4, true, true);
    wuma->addSkill(new YearWuma);
    wuma->addSkill("yearruishou");

    General *weiyang = new General(this, "bossweiyang", "god", 3, false, true);
    weiyang->addSkill(new YearWeiyang);
    weiyang->addSkill(new YearChuanchengWeiyang);
    weiyang->addSkill("yearruishou");

    General *shenhou = new General(this, "bossshenhou", "god", 3, true, true);
    shenhou->addSkill(new YearShenhou);
    shenhou->addSkill(new YearChuanchengShenhou);
    shenhou->addSkill("yearruishou");

    General *youji = new General(this, "bossyouji", "god", 3, true, true);
    youji->addSkill(new YearYouji);
    youji->addSkill("yearruishou");

    General *xugou = new General(this, "bossxugou", "god", 4, true, true);
    xugou->addSkill(new YearXugou);
    xugou->addSkill(new YearXugouTargetMod);
    xugou->addSkill(new YearChuanchengXugou);
    xugou->addSkill("yearruishou");

    General *haizhu = new General(this, "bosshaizhu", "god", 5, true, true);
    haizhu->addSkill(new YearHaizhu);
    haizhu->addSkill("yearruishou");

    General *easyyear = new General(this, "easy_boss_year", "god", 6, true, true);
    easyyear->addSkill(new YearJiyuan);
    easyyear->addSkill(new YearSuizhongEasy);
    easyyear->addSkill(new YearCuikuEasy);

    General *year = new General(this, "boss_year", "god", 8, true, true);
    year->addSkill("yearjiyuan");
    year->addSkill(new YearNianyi);
    year->addSkill(new YearNianyiTargetMod);
    year->addSkill(new YearSuizhong);
    year->addSkill(new YearCuiku);

    General *diyear = new General(this, "difficult_boss_year", "god", 10, true, true);
    diyear->addSkill("yearjiyuan");
    diyear->addSkill(new YearNianyiDifficult);
    diyear->addSkill(new YearNianyiDifficultEffect);
    diyear->addSkill("#yearnianyi-target");
    diyear->addSkill("yearsuizhong");
    diyear->addSkill(new YearCuikuDifficult);

    addMetaObject<YearZishuCard>();
    addMetaObject<YearYinhuCard>();
    addMetaObject<YearChenlongCard>();
    addMetaObject<YearWeiyangCard>();
}

ADD_PACKAGE(YearBoss)
