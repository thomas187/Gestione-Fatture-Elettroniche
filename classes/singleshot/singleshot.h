#ifndef SINGLESHOT_H
#define SINGLESHOT_H

#include <QObject>
#include <QVariant>

#include <RepoTimer>

#ifndef Q_WAIT_INTERVAL
#define Q_WAIT_INTERVAL 300 // msec
#endif // Q_WAIT_INTERVAL


#ifndef Q_WAIT_AND_SEND
#define Q_WAIT_AND_SEND(key, func) \
      singleShot(key, Q_WAIT_INTERVAL, this, [=](QVariant, int, QObject*){ \
        this->func(); \
      });
#endif // Q_WAIT_AND_SEND

#ifndef Q_WAIT_INTERVAL_AND_SEND
#define Q_WAIT_INTERVAL_AND_SEND(interval, key, func) \
      singleShot(key, interval, this, [=](QVariant, int, QObject*){ \
        this->func(); \
      });
#endif // Q_WAIT_INTERVAL_AND_SEND


#ifndef Q_WAIT_AND_SEND_ARG
#define Q_WAIT_AND_SEND_ARG(key, func, arg) \
      singleShot(key, Q_WAIT_INTERVAL, this, [=](QVariant, int, QObject*){ \
        this->func(arg); \
      });
#endif // Q_WAIT_AND_SEND_ARG


#ifndef Q_WAIT_INTERVAL_AND_SEND_ARG
#define Q_WAIT_INTERVAL_AND_SEND_ARG(interval, key, func, arg) \
      singleShot(key, interval, this, [=](QVariant, int, QObject*){ \
        this->func(arg); \
      });
#endif // Q_WAIT_INTERVAL_AND_SEND_ARG


class Singleshot
{
public:
    Singleshot();
    ~Singleshot();

public:
    // Single shot
    // -------------------------------------------------- //
    void clearSingleShot(){
        core::timer::stop(t_singleShot);
        t_singleShot = nullptr;
        for(auto t : t_map_tSingleshot.toStdMap()){
            core::timer::stop(t.second);
            t_map_tSingleshot.remove(t.first);
        }

    }
    void clearSingleShot(QVariant key){
        if(t_map_tSingleshot.contains(key.toString())){
            QTimer* t = t_map_tSingleshot.value(key.toString());
            core::timer::stop(t);
            t_map_tSingleshot.remove(key.toString());
        }
    }
    // -------------------------------------------------- //


    // Single shot
    // -------------------------------------------------- //
protected:
    QTimer *t_singleShot = nullptr;
    template <typename T, typename W = QObject>
    void singleShot(int timeout, W *parent, T func)
    {
        core::timer::stop(t_singleShot);
        t_singleShot = core::timer::start(timeout,parent,true);
        QObject::connect(t_singleShot, &QTimer::timeout, [=]{
            func(timeout,parent);
            core::timer::stop(t_singleShot);
            t_singleShot = nullptr;
        });
    }
    // -------------------------------------------------- //



    // Single shot
    // -------------------------------------------------- //
protected:
    QMap<QString,QTimer*> t_map_tSingleshot;
    template <typename T, typename W = QObject>
    void singleShot(QVariant key, int timeout, W *parent, T func)
    {
        if(!t_map_tSingleshot.keys().contains(key.toString()))
            t_map_tSingleshot.insert(key.toString(),nullptr);
        QTimer *t = t_map_tSingleshot.value(key.toString());
        core::timer::stop(t);
        t = core::timer::start(timeout,parent,true);
        t_map_tSingleshot.insert(key.toString(),t);
        QObject::connect(t, &QTimer::timeout, [=]{
            func(key, timeout, parent);
            core::timer::stop(t);
            t_map_tSingleshot.remove(key.toString());
        });
    }
    // -------------------------------------------------- //
};

#endif // SINGLESHOT_H
