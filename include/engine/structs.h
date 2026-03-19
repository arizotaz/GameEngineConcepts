// #############################################################################
// # structs.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       02/10/2025
// # Last Modification:  03/10/2025
// #############################################################################
// # List of different structures used in the Engine
// #############################################################################
// #
// # This header contains many different structures that all perform different
// # actions.
// #
// # Menu       - A structure to hold information about different UI Menus,
// #              Update, and Renderering calls.  Provides hooks for changing
// #              to and changing from menus
// # MenuManger - A structure to register Menus and handle changing between
// #              menus
// # Process    - Similar to menu, provides very simple standard Draw, Update
// #              Start, and Exit calls.  Easy to change out Processes
// # Vector2<T,T> - Simple duple type object to store two values in a single
// #                structure
// # Vector3<T,T,T> - Simple object to store three values in a single structure
// # Rect<T,T,T,T> - Simple object to store four values in a single structure
// # DeltaTime  - Simple structure to provide methods for DeltaTime
// #
// #############################################################################

#ifndef ENGINE_STRUCS_H
#define ENGINE_STRUCS_H 1

namespace GEC {

template <typename V1, typename V2, typename V3, typename V4>
class Rect;
class MenuManager;

/**
 * Structure to represent a process
 * Provides a simple and standard way to address the common function of an
 * application
 */
class Process {
public:
    /**
     * Create a process and register ID
     */
    Process(int id) { _processID = id; }

    /**
     * Called when a process starts
     */
    void virtual Start() = 0;

    /**
     * Update loop
     */
    void virtual Update() = 0;

    /**
     * Called when a process exits
     */
    void virtual Exit() = 0;

    /**
     * Render Loop
     */
    void virtual Render() = 0;

    /**
     * GUI Event Calls
     */
    void virtual Events() = 0;

private:
    int _processID;
};

class Menu {
public:
    /**
     * Called when a menu is first opened
     */
    virtual void Open() { };

    /** Menu Update Loop */
    virtual void Update() { };

    /** Menu Render Loop */
    virtual void Render() { };

    /** Handle Events */
    virtual void Events() { };

    /** Called when the menu is closed */
    virtual void Leave() { };

    /** Sets the manager of this menu */
    virtual void SetManager(MenuManager* mm) final
    {
        this->manager = mm;
    };

    /** Returns the manager of this menu */
    virtual MenuManager* GetManager() const
    {
        return this->manager;
    };

private:
    MenuManager* manager;
};

/** Menu Manager
 * Used to handle addition, process, render, and changing of menus
 *
 * Use in this order
 * MenuManager* mm = new MenuManager();
 *
 * mm->Update()
 * mm->Events()
 * mm->Render()
 *
 */
class MenuManager {
public:
    /**
     * Initialize the menu manager
     */
    MenuManager();

    /** Update Loop */
    void Update();
    /** Render Loop */
    void Render();
    /** Process Input Events */
    void Events();

    /** Add Menu to registry
     * @param id - Menu ID
     * @param menu - The new Menu Pointer
     */
    void AddMenu(int id, Menu* menu);

    bool Exists(int id);

    void RemoveMenu(int id);

    /**
     * Returns the menu registered at id
     * @param id - the Menu ID
     * @return Menu* - Pointer to the menu or nullptr
     */
    Menu* GetMenu(int id);

    /**
     * Tells the menu manager to go to a new menu
     * @param id - the new Menu ID
     */
    void GoTo(int id)
    {
        this->nextIndex = id;
    };

    /**
     * Destructor obv
     */
    ~MenuManager();

private:
    Menu** menuList;
    int menus = 0;

    Menu* currMenu = nullptr;
    int nextIndex = 0, currInd = -1, lastInd = -1;
};

/** Vector2 - Simple duple element object */
template <typename V1, typename V2>
class Vector2 {
public:
    /** Initialize Vector2 with 2 variables
     * @param v1 - first item
     * @param v2 - second item
     */
    Vector2(V1 v1, V2 v2)
        : _v1(v1)
        , _v2(v2)
    {
    }

    /** Initialize Vector2 as a copy of another vector
     * @param other - vector to copy from
     */
    Vector2(const Vector2& other)
        : _v1(other._v1)
        , _v2(other._v2)
    {
    }

    /** Initialize Vector2 as a copy of another vector
     * @param other - vector to copy from
     */
    Vector2& operator=(const Vector2& other)
    {
        if (this != &other) {
            _v1 = other._v1;
            _v2 = other._v2;
        }
        return *this;
    }

    /** Return the first value
     * @return _v1 - the first element
     */
    V1& First()
    {
        return _v1;
    }
    /** Return the second value
     * @return _v2 - the second element
     */
    V2& Second()
    {
        return _v2;
    }

    /** Return the first value
     * @return _v1 - the first element
     */
    const V1& First() const
    {
        return _v1;
    }
    /** Return the second value
     * @return _v2 - the second element
     */
    const V2& Second() const
    {
        return _v2;
    }

    /** Moves the values of the vector by (v1,v2)
     * @param v1 - how much to add to the x value
     * @param v2 - how much to add to the y value
     */
    void Move(V1 v1, V2 v2)
    {
        _v1 += v1;
        _v2 += v2;
    }

    void Move(Vector2<V1, V2> val)
    {
        _v1 += val._v1;
        _v2 += val._v2;
    }

    /** Sets the values of the vector to (v1,v2)
     * @param v1 - the new x value
     * @param v2 - the new y value
     */
    void Set(V1 v1, V2 v2)
    {
        _v1 = v1;
        _v2 = v2;
    }

    Vector2<V1, V2> operator-() const
    {
        return Vector2<V1, V2>(-_v1, -_v2);
    }

    Vector2<V1, V2> operator*(const int val) const
    {
        return Vector2<V1, V2>(_v1 * val, _v2 * val);
    }
    Vector2<V1, V2> operator*(const float val) const
    {
        return Vector2<V1, V2>(_v1 * val, _v2 * val);
    }
    Vector2<V1, V2> operator*(const Vector2& val) const
    {
        return Vector2<V1, V2>(_v1 * val._v1, _v2 * val._v2);
    }

    Vector2<V1, V2> operator/(const int val) const
    {
        return Vector2<V1, V2>(_v1 / val, _v2 / val);
    }
    Vector2<V1, V2> operator/(const float val) const
    {
        return Vector2<V1, V2>(_v1 / val, _v2 / val);
    }
    Vector2<V1, V2> operator/(const Vector2& val) const
    {
        return Vector2<V1, V2>(_v1 / val._v1, _v2 / val._v2);
    }

private:
    V1 _v1;
    V2 _v2;
};

/** Vector2 - Simple triple element object */
template <typename V1, typename V2, typename V3>
class Vector3 {
public:
    /** Initialize Vector3 with 3 variables
     * @param v1 - first item
     * @param v2 - second item
     * @param v3 - third item
     */
    Vector3(V1 v1, V2 v2, V3 v3)
        : _v1(v1)
        , _v2(v2)
        , _v3(v3)
    {
    }

    /** Initialize Vector3 as a copy of another vector
     * @param other - vector to copy from
     */
    Vector3(const Vector3& other)
        : _v1(other._v1)
        , _v2(other._v2)
        , _v3(other._v3)
    {
    }

    /** Initialize Vector3 as a copy of another vector
     * @param other - vector to copy from
     */
    Vector3& operator=(const Vector3& other)
    {
        if (this != &other) {
            _v1 = other._v1;
            _v2 = other._v2;
            _v3 = other._v3;
        }
        return *this;
    }

    /** Return the first value
     * @return _v1 - the first element
     */
    V1 First() const
    {
        return _v1;
    }
    /** Return the second value
     * @return _v2 - the second element
     */
    V2 Second() const
    {
        return _v2;
    }
    /** Return the third value
     * @return _v3 - the third element
     */
    V2 Third() const
    {
        return _v3;
    }

    /** Moves the values of the vector by (v1,v2,v3)
     * @param v1 - how much to add to the x value
     * @param v2 - how much to add to the y value
     * @param v3 - how much to add to the z value
     */
    void Move(V1 v1, V2 v2, V3 v3)
    {
        _v1 += v1;
        _v2 += v2;
        _v3 += v3;
    }

    /** Sets the values of the vector to (v1,v2,v3)
     * @param v1 - the new x value
     * @param v2 - the new y value
     * @param v3 - the new z value
     */
    void Set(V1 v1, V2 v2, V3 v3)
    {
        _v1 = v1;
        _v2 = v2;
        _v3 = v3;
    }

private:
    V1 _v1;
    V2 _v2;
    V3 _v3;
};

template <typename V1, typename V2, typename V3, typename V4>
class Rect {
public:
    /** Initialize Rect with 4 variables
     * @param x - x value
     * @param y - y value
     * @param w - width value
     * @param h - height value
     */
    Rect(V1 x, V2 y, V3 w, V4 h)
        : _first(x, y)
        , _second(w, h)
    {
    }

    /** Initialize Rect with 2 vectors
     * @param first - position vector
     * @param second - size vector
     */
    Rect(Vector2<V1, V2> first, Vector2<V3, V4> second)
        : _first(first)
        , _second(second)
    {
    }

    /** Initialize Rect as a copy of another Rect
     * @param other - Rect to copy from
     */
    Rect(const Rect& other)
        : _first(other._first)
        , _second(other._second)
    {
    }

    /** Initialize Rect as a copy of another Rect
     * @param other - Rect to copy from
     */
    Rect& operator=(const Rect& other)
    {
        if (this != &other) {
            _first = other._first;
            _second = other._second;
        }
        return *this;
    }

    /** Return the position vector
     * @return position_vector
     */
    Vector2<V1, V2>& First()
    {
        return _first;
    }
    /** Return the size vector
     * @return size_vector
     */
    Vector2<V3, V4>& Second()
    {
        return _second;
    }

    /** Returns the x position
     * @return x
     */
    V1& X()
    {
        return _first.First();
    }
    /** Returns the y position
     * @return y
     */
    V2& Y()
    {
        return _first.Second();
    }
    /** Returns the width of the rect
     * @return width
     */
    V3& W()
    {
        return _second.First();
    }
    /** Returns the height of the rect
     * @return height
     */
    V4& H()
    {
        return _second.Second();
    }

private:
    Vector2<V1, V2> _first;
    Vector2<V3, V4> _second;
};

/** Simple Deltatime structure
 * Exposes .Update(), this structure computes time between each .Update()
 * call and returns that number with .Get()
 */
class DeltaTime {
public:
    DeltaTime();

    /**
     * Returns the last amount of time between .Update() calls
     */
    float Get();
    /**
     * Computes the time since the last .Update() call
     */
    void Update();

private:
    int lastTime = 0;
    float dtime = 0;
};

}

#endif