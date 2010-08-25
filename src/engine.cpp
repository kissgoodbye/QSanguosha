#include "engine.h"
#include "card.h"
#include "standard.h"
#include "client.h"

#include <QFile>
#include <QStringList>
#include <QMessageBox>
#include <QDir>

Engine *Sanguosha = NULL;

Engine::Engine(QObject *parent)
    :QObject(parent), effect(Phonon::createPlayer(Phonon::MusicCategory))
{
    addPackage(new StandardPackage);

    metaobjects.insert("NamePattern", &NamePattern::staticMetaObject);
    metaobjects.insert("TypePattern", &TypePattern::staticMetaObject);
    metaobjects.insert("ClassPattern", &ClassPattern::staticMetaObject);
}

void Engine::addPackage(Package *package){
    package->setParent(this);

    QList<Card *> all_cards = package->findChildren<Card *>();
    foreach(Card *card, all_cards){
        card->setId(cards.length());
        cards << card;
        metaobjects.insert(card->objectName(), card->metaObject());
    }

    QList<General *> all_generals = package->findChildren<General *>();
    foreach(General *general, all_generals){
        if(general->isLord())
            lord_list << general->objectName();
        else
            nonlord_list << general->objectName();

        generals.insert(general->objectName(), general);

        QList<const Skill *> all_skills = general->findChildren<const Skill *>();
        foreach(const Skill *skill, all_skills)
            skills.insert(skill->objectName(), skill);
    }

    QList<const QMetaObject *> metas = package->getMetaObjects();
    foreach(const QMetaObject *meta, metas)
        metaobjects.insert(meta->className(), meta);

    translations.unite(package->getTranslation());
}

QString Engine::translate(const QString &to_translate) const{
    return translations.value(to_translate, to_translate);
}

const General *Engine::getGeneral(const QString &name) const{
    return generals.value(name, NULL);
}

int Engine::getGeneralCount() const{
    return generals.size();
}

const Card *Engine::getCard(int index) const{
    if(index < 0 || index >= cards.length())
        return NULL;
    else
        return cards[index];
}

Card *Engine::cloneCard(const QString &name, Card::Suit suit, int number) const{
    const QMetaObject *meta = metaobjects.value(name, NULL);
    if(meta){
        QObject *card_obj = meta->newInstance(Q_ARG(Card::Suit, suit), Q_ARG(int, number));
        return qobject_cast<Card *>(card_obj);
    }else
        return NULL;
}

SkillCard *Engine::cloneSkillCard(const QString &name){
    const QMetaObject *meta = metaobjects.value(name, NULL);
    if(meta){
        QObject *card_obj = meta->newInstance();
        SkillCard *card = qobject_cast<SkillCard *>(card_obj);
        return card;
    }else
        return NULL;
}

CardPattern *Engine::cloneCardPattern(const QStringList &captured_texts){
    QString name = captured_texts.at(1);
    QString pattern_str = captured_texts.at(2);
    int min = captured_texts.at(3).toInt();
    int max = captured_texts.at(4).toInt();
    QString flags = captured_texts.at(5);

    const QMetaObject *meta = metaobjects.value(name, NULL);
    if(meta){
        QObject *pattern_obj = meta->newInstance(Q_ARG(QString, pattern_str));
        CardPattern *pattern = qobject_cast<CardPattern *>(pattern_obj);
        if(pattern){
            pattern->min = min;
            pattern->max = max;

            static QChar compulsory_symbol('c');
            static QChar response_symbol('r');
            if(flags.contains(compulsory_symbol))
                pattern->compulsory = true;
            if(flags.contains(response_symbol))
                pattern->response = true;

            return pattern;
        }
    }

    return NULL;
}

int Engine::getCardCount() const{
    return cards.length();
}

QStringList Engine::getRandomLords(int lord_count) const{
    QStringList lords = lord_list;

    if(lord_count < lord_list.length()){
        QMessageBox::warning(NULL, tr("Warning"),
                             tr("The lord count must greater or equal to the intrinsic lord number(%1)").arg(lord_list.length()));
        return lord_list;
    }

    QStringList nonlord_list = this->nonlord_list;
    int i, n = nonlord_list.length();
    for(i=0; i<n; i++){
        int r1 = qrand() % n;
        int r2 = qrand() % n;
        nonlord_list.swap(r1, r2);
    }

    int extra = lord_count - lord_list.length();
    for(i=0; i< extra; i++)
        lords << nonlord_list.at(i);

    return lords;
}

QStringList Engine::getRandomGenerals(int count, const QSet<QString> &ban_set) const{
    QStringList all_generals = generals.keys();

    int n = all_generals.count();
    Q_ASSERT(n >= count);

    // shuffle them
    int i;
    for(i=0; i<n; i++){
        int r1 = qrand() % n;
        int r2 = qrand() % n;
        all_generals.swap(r1, r2);
    }

    if(!ban_set.isEmpty()){
        QSet<QString> general_set = all_generals.toSet();
        all_generals = general_set.subtract(ban_set).toList();
    }

    QStringList general_list = all_generals.mid(0, count);
    Q_ASSERT(general_list.count() == count);

    return general_list;
}

QList<int> Engine::getRandomCards() const{
    QList<int> list;
    int n = cards.count(), i;
    for(i=0; i<n; i++)
        list << i;

    for(i=0; i<n; i++){
        int r1 = qrand() % n;
        int r2 = qrand() % n;
        list.swap(r1, r2);
    }

    return list;
}

void Engine::playEffect(const Phonon::MediaSource &source){
    effect->setCurrentSource(source);
    effect->play();
}

void Engine::playSkillEffect(const QString &skill_name, int index){
    const Skill *skill = skills.value(skill_name, NULL);
    if(skill)
        skill->playEffect(index);
}

const Skill *Engine::getSkill(const QString &skill_name) const{
    return skills.value(skill_name, NULL);
}