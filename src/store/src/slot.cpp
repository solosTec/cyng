#include <cyng/store/slot.h>
#include <cyng/store/table.h>

namespace cyng {

    slot::slot(channel &ch, channel::cb_err_t cb)
        : channel_(ch.weak_from_this())
        , cb_(cb) {}

    slot::slot(channel_ptr cp, channel::cb_err_t cb)
        : channel_(cp.get()->weak_from_this())
        , cb_(cb) {}

    slot::slot(channel_weak ch, channel::cb_err_t cb)
        : channel_(ch)
        , cb_(cb) {}

    slot::slot(slot const &s)
        : channel_(s.channel_)
        , cb_(s.cb_) {}

    bool slot::forward(table const *tbl, key_t const &key, data_t const &data, std::uint64_t gen, boost::uuids::uuid tag) {

        auto sp = channel_.lock();
        if (sp)
            sp->dispatch("db.insert", cb_, cyng::make_tuple(tbl, key, data, gen, tag));

        return sp.operator bool();
    }

    bool slot::forward(
        table const *tbl,
        key_t const &key,
        attr_t const &attr,
        data_t const &data,
        std::uint64_t gen,
        boost::uuids::uuid tag) {

        auto sp = channel_.lock();

        if (sp)
            sp->dispatch(
                "db.modify",
                cb_,
                cyng::make_tuple(
                    tbl,
                    //			tbl->meta().get_name(),
                    key,
                    attr,
                    gen,
                    tag));

        return sp.operator bool();
    }

    bool slot::forward(table const *tbl, key_t const &key, data_t const &data, boost::uuids::uuid tag) {

        auto sp = channel_.lock();

        //	remove event
        if (sp)
            sp->dispatch("db.remove", cb_, cyng::make_tuple(tbl, key, tag));

        return sp.operator bool();
    }

    bool slot::forward(table const *tbl, boost::uuids::uuid tag) {

        auto sp = channel_.lock();
        if (sp)
            sp->dispatch("db.clear", cb_, cyng::make_tuple(tbl, tag));
        return sp.operator bool();
    }

    bool slot::forward(table const *tbl, bool trx) {

        auto sp = channel_.lock();
        if (sp)
            sp->dispatch("db.trx", cb_, cyng::make_tuple(tbl, trx));
        return sp.operator bool();
    }

    slot_ptr make_slot(channel &ch, channel::cb_err_t cb) { return std::make_shared<slot>(ch, cb); }
    slot_ptr make_slot(channel_ptr cp, channel::cb_err_t cb) { return std::make_shared<slot>(cp, cb); }
    slot_ptr make_slot(channel_weak cw, channel::cb_err_t cb) {
        auto cp = cw.lock();
        return (cp) ? make_slot(cp, cb) : slot_ptr();
    }

} // namespace cyng
