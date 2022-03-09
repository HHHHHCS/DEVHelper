#pragma once

#include <memory>

#include <QThread>
#include <QQmlApplicationEngine>


namespace managers { class LinkManager; }

namespace communication
{
    class Proto : public QThread
    {
        Q_OBJECT

        friend class managers::LinkManager;

        signals:
            void sigShakehand();
            void sigBreakdown();
            void sigParsed(Proto*, QByteArray&);
            void sigPacked(Proto*, QByteArray&);

        private slots:
            virtual void slotParseProto(const QByteArray&) = 0;
            virtual void slotPackProto(const QByteArray&) = 0;

        public:
            enum class ProtoType : int8_t
            {
                UNKNOWN_TYPE = -1,
                MAVLINK,
                // TODO(huangchsh): 后续协议类型在此添加
            };

        public:
            virtual ~Proto() = default;

            virtual void setProtoName(const QString name) = 0;
            virtual QString getProtoName() const = 0;

        protected:
            Proto(ProtoType proto_type)
                : QThread(nullptr)
                , m_proto_type(proto_type)
            {
                QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
                qRegisterMetaType<Proto*>("communication::Proto");
            }

            ProtoType getLinkType() const { return m_proto_type; }

        private:
            const ProtoType m_proto_type;
    };
    using UniquePtrProto = std::unique_ptr<Proto>;
    using SharedPtrProto = std::shared_ptr<Proto>;
    using WeakPtrProto = std::weak_ptr<Proto>;
}   // namespace communication