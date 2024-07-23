#ifndef PROPERTYMANAGER_H
#define PROPERTYMANAGER_H

#include <QMutex>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFutureWatcher>
#include <QtConcurrent>
#include <QFileDialog>
#include <QUndoStack>

#include "Nodes/model.h"
#include "Components/componentbase.h"
#include "Manipulator/manipulatortranslate.h"
#include "Manipulator/manipulatorscale.h"
#include "Manipulator/manipulatorrotate.h"
#include "propertyswgt.h"
#include "nodestreewgt.h"
#include "nodeeditwgt.h"
#include "ToolWgts/loadingwgt.h"
#include "configfile.h"
#include "UndoCommand/undocommands.h"

#define g_PropertyManager PropertyManager::getPropertyManager()

#define SHOWAABB    "showAABB"
#define SHOWMESH    "showMesh"
#define SHOWOUTLINE "showOutline"

class PropertyManager :public QObject
{
    Q_OBJECT
public:
    static PropertyManager* getPropertyManager();

    ConfigFile   *config()      {return m_pConfig;}
    PropertysWgt *propertysWgt(){return m_pPropertysWgt;}
    NodesTreeWgt *nodesTreeWgt(){return m_pNodesTreeWgt;}
    NodeEditWgt  *nodeEditWgt() {return m_pNodeEditWgt;}
    LoadingWgt   *loadWgt()     {return m_pLoadWgt;}
    Camera       *camera()      {return m_pCamera;}
    QUndoStack   *undoStack()   {return m_pUndoStack;}

    QVariantMap nodeSelectState(){return m_nodeSelectState;}
    uint getTextureId(const QString &path, bool addtoProperty = true);
    Node *loadModel(const QString &path, bool addtoProperty = true);
    Node *copyNode(Node *node);
    Node *loadNodeByData(QVariantMap data);

    QList<Node *> nodes(){return m_nodes;}
    void addNode(Node *node, Node *parent = nullptr);
    void delNode(Node *node);
    void selectNode(Node *node);
    void setNodeSelectState(QVariantMap nodeSelectState){
        m_nodeSelectState = nodeSelectState;
        m_pConfig->Set("nodeSelectState", SHOWAABB, m_nodeSelectState.value(SHOWAABB));
        m_pConfig->Set("nodeSelectState", SHOWMESH, m_nodeSelectState.value(SHOWMESH));
        m_pConfig->Set("nodeSelectState", SHOWOUTLINE, m_nodeSelectState.value(SHOWOUTLINE));
        selectNode(selectNode());
    }

    Node *selectNode(){return m_pSelectNode;}

    void setIsDrawMesh(bool is);
    void setFloor(Node *floor){m_pFloor = floor;}
    Node* floor(){return m_pFloor;}
    void setManipulator(ManipulatorBase * manipulator){m_pManipulator = manipulator;}
    ManipulatorBase *manipulator(){return m_pManipulator;}
    void setManipulatorType(MANIPULATOR_TYPE type);
    void setManipulatorReferenceSystem(MANIPULATOR_REFERENCE_SYSTEM referenceSystem);
    void removeAll();

    void addSelectTmpNode(float len, Node *node);
    QMap<float, Node *> selectTmpNodes(){return m_selectTmpNodes;}
    void removeSelectTmpNodes(){m_selectTmpNodes.clear();}
    void setFps(double fps){m_fps = fps;}
    double fps(){return m_fps;}
    void openWorld(const QString &world);
    void saveWorld(QString path = "");
    void setIsNeedSave(bool is){m_isNeedSave = is;}
    bool isNeedSave(){return m_isNeedSave;}

    bool m_isLoadWorld = false;
    QList<Node *> m_tmpNodes;

signals:
    void renderLogic();
    void setEnabledSignal(bool);
    void setTaskbarProgressValue(int value);

private:
    PropertyManager();

private:
    static PropertyManager *m_propertyManager;

    QUndoStack *m_pUndoStack               = nullptr;
    ConfigFile *m_pConfig                  = nullptr;
    PropertysWgt *m_pPropertysWgt          = nullptr;
    NodesTreeWgt *m_pNodesTreeWgt          = nullptr;
    NodeEditWgt  *m_pNodeEditWgt           = nullptr;
    LoadingWgt *m_pLoadWgt                 = nullptr;
    Camera *m_pCamera                      = nullptr;
    ManipulatorBase *m_pManipulator        = nullptr;
    ManipulatorTranslate *m_pManipulator_t = nullptr;
    ManipulatorRotate *m_pManipulator_r    = nullptr;
    ManipulatorScale *m_pManipulator_s     = nullptr;
    Node *m_pFloor                         = nullptr;
    Node *m_pSelectNode                    = nullptr;

    QList<Node *> m_nodePropertys;
    vector<Texture> m_texturePropertys;
    QList<Node *> m_nodes;
    QMap<float, Node *> m_selectTmpNodes;
    bool m_isDrawMesh = false;
    QString m_worldPath = "";
    double m_fps = 60;
    bool m_isNeedSave = false;
    QVariantMap m_nodeSelectState;
};

#endif // PROPERTYMANAGER_H
