#ifndef MENUITEM_H  
#define MENUITEM_H  

#include <QObject>  

class MenuItem  
{  
public:  
    MenuItem(int iMenuId, const QString &strMenuName, bool bIsChecked = false);  
    MenuItem();  
    ~MenuItem();  

    MenuItem(const MenuItem &menuItem);  
    int GetMenuId() const;  
    void SetMenuId(int iValue);  

    QString GetMenuName() const;  
    void SetMenuName(const QString &strValue);  
  
    bool GetIsChecked() const;  
    void SetIsChecked(bool bValue);  

private:  
    int     m_iMenuId;  
    QString m_strMenuName;  
    bool    m_bIsChecked;  
};  

#endif // MENUITEM_H  
