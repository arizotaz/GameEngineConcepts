#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H 1

#include <engine/game_structs.h>
#include <engine/input.h>
#include <engine/structs.h>
#include <game/gameprocessor.h>
#include <game/level.h>

#include <engine/ui/element.h>
#include <engine/ui/elements.h>
#include <game/tile.h>

class TileRenderer : public GEC::Game::GameObject {
public:
    TileRenderer()
        : GameObject("com.arizotaz.gec.tilerenderer")
        , m_pos(0, 0)
        , cursor(0, 0)
    {
        this->name = "Untitled Tile Renderer";
        levelData = new Level("EditorLevel");
        levelData->Init(100, 100, 2);
        lr = new LevelRenderer(levelData);
        this->persistent = true;
    }
    void Start() override { }
    void Update() override
    {
        if (editMode) {
            if (leftMouseDown)
                levelData->SetTile(selectedDrawTile, cursor.First(), cursor.Second(), brushLayer);
        } else
            leftMouseDown = false;
    }
    void Render() override
    {
        GEC::Camera& cam = GEC::Camera::GetInstance();
        cursor = GEC::Vector2<int, int>(
            round(m_pos.First() / cam.GetScale() + cam.Position().First()),
            round(m_pos.Second() / cam.GetScale() + cam.Position().Second()));

        if (drawLayer < 0)
            lr->Render();
        else
            lr->DrawLayer(drawLayer, levelData->GetLayerData(drawLayer)->zDepth);

        if (editMode) {
            GEC::Render::SetColor(255, 0, 0, 100);
            GEC::Render::Rect(cursor.First(), cursor.Second(), 1, 1);
        } else {
            drawLayer = -1;
        }
        editMode = false;
    }
    ~TileRenderer() { }

    virtual GameObject* Clone() const override { return nullptr; }

    void UpdatePropertiesPanel(float x, float y, float width, float height) override
    {
        editMode = true;

        GEC::Camera& cam = GEC::Camera::GetInstance();
        GEC::Input::Mouse& mou = GEC::Input::Mouse::GetInstance();

        m_pos = mou.Position();

        m_pos.Set(m_pos.First() - cam.ViewPort().First() / 2, cam.ViewPort().Second() / 2 - m_pos.Second());

        // Create Buttons;
        if (tileSelect.size() == 0) {
            labels.push_back(new GEC::UI::Elements::TextDisplay("Tile Select"));
            for (int i = 0; i < 4; ++i)
                tileSelect.push_back(new GEC::UI::Elements::Button());
        }
        if (layerSelect.size() == 0) {
            for (int i = 0; i < levelData->Layers() + 1; ++i)
                layerSelect.push_back(new GEC::UI::Elements::Button());
        }
        if (brushLayerInput == nullptr) {
            brushLayerInput = new GEC::UI::Elements::InputField();
            brushLayerInput->SetValue("0");
            labels.push_back(new GEC::UI::Elements::TextDisplay("Brush Layer"));
        }

        if (layerCollidable_input == nullptr) {
            labels.push_back(new GEC::UI::Elements::TextDisplay("Layer Data"));
            layerCollidable_input = new GEC::UI::Elements::Checkbox();
        }

        float bSize = 15;
        GEC::Vector2<float, float> iInd(x, y + height / 2 - bSize / 2);

        for (int i = 0; i < layerSelect.size(); ++i) {
            std::string bText = "Draw Layer " + std::to_string(i - 1);

            if (i == 0)
                bText = "Draw All Layers";
            layerSelect[i]->Set(bText, iInd.First(), iInd.Second(), width, bSize);
            iInd.Move(0, -bSize);

            tileSelect[i]->Update();
        }
        iInd.Move(0, -bSize);

        bSize = 25;

        iInd.Set(x - width / 2, iInd.Second());

        labels[0]->Set(x - width / 2, iInd.Second(), width, 15);
        iInd.Move(0, -15);
        for (int i = 0; i < tileSelect.size(); ++i) {
            iInd.Move(bSize / 2, 0);
            tileSelect[i]->Set(iInd.First(), iInd.Second(), bSize, bSize);
            iInd.Move(bSize / 2 + 5, 0);
            if (iInd.First() + bSize > x + width / 2)
                iInd.Set(x - width / 2, iInd.Second() + bSize + 5);

            tileSelect[i]->Update();
        }
        iInd.Set(x, iInd.Second() - bSize * 1.5f);

        labels[1]->Set(x - width / 2, iInd.Second(), width, 15);
        iInd.Move(0, -15);
        bSize = 20;
        brushLayerInput->Set(iInd.First(), iInd.Second(), width, bSize);
        brushLayerInput->Update();
        iInd.Move(0, -bSize * 1.5f);

        labels[2]->Set(x - width / 2, iInd.Second(), width, 15);
        iInd.Move(0, -15);
        bSize = 20;
        layerCollidable_input->Set("Layer is Collidable", x - width / 2, iInd.Second(), width, bSize);
        layerCollidable_input->Update();
        iInd.Move(0, -bSize * 1.5f);

        for (auto l : labels) {
            l->Update();
        }
    }
    void InteractPropertiesPanel(float x, float y, float width, float height) override
    {
        for (int i = 0; i < tileSelect.size(); ++i) {
            tileSelect[i]->Interact();
            if (tileSelect[i]->Clicked())
                selectedDrawTile = i;
        }

        for (int i = 0; i < layerSelect.size(); ++i) {
            layerSelect[i]->Interact();
            if (layerSelect[i]->Clicked()) {
                drawLayer = i - 1;
                if (drawLayer > -1) {
                    brushLayerInput->SetValueAsUser(std::to_string(i - 1));
                }
            }
        }

        if (brushLayerInput != nullptr) {
            brushLayerInput->Interact();
            if (brushLayerInput->Changed()) {
                this->brushLayer = std::stoi(brushLayerInput->GetValue());
                layerCollidable_input->SetValue(levelData->GetLayerData(drawLayer)->collidable);
            }
        }

        for (auto l : labels) {
            l->Interact();
        }
        layerCollidable_input->Interact();
        if (layerCollidable_input->Changed()) {
            levelData->GetLayerData(brushLayer)->collidable = layerCollidable_input->GetValue();
        }
    }
    void RenderPropertiesPanel(float x, float y, float width, float height) override
    {
        // Detect Mouse Down
        if (!GEC::UI::ElementRegistry::GetInstance().ShouldStopInteract()) {
            if (GEC::Input::Mouse::GetInstance().LeftDown()) {
                leftMouseDown = true;
            }
        }
        if (!GEC::Input::Mouse::GetInstance().LeftDown())
            leftMouseDown = false;

        for (int i = 0; i < tileSelect.size(); ++i) {
            tileSelect[i]->Render();

            Tile* t = TileList::GetInstance().GetTile(i);

            glPushMatrix();
            glTranslatef(tileSelect[i]->X(), tileSelect[i]->Y(), 0);
            glScalef(tileSelect[i]->Width(), tileSelect[i]->Height(), 0);
            t->Render(lr, 0, 0, 0);
            glPopMatrix();
        }

        for (auto l : labels) {
            l->Render();
        }

        for (int i = 0; i < layerSelect.size(); ++i) {
            layerSelect[i]->Render();
        }

        if (brushLayerInput != nullptr)
            brushLayerInput->Render();

        if (layerCollidable_input != nullptr)
            layerCollidable_input->Render();
    }

    Level* LevelData()
    {
        return levelData;
    }

    void WriteObject(std::ostream& out) const override
    {
        levelData->Serialize(out);
    }
    void ReadObject(std::istream& in) override {
        if (levelData != nullptr) {
            delete levelData;
        }

        levelData = new Level("");
        levelData->Deserialize(in);
        delete lr;
        lr = new LevelRenderer(levelData);
    }

protected:
    LevelRenderer* lr;
    Level* levelData = nullptr;

    // Editor vars
    GEC::Vector2<float, float> m_pos;
    GEC::Vector2<int, int> cursor;
    bool editMode = false, leftMouseDown = false;
    std::vector<GEC::UI::Elements::Button*> tileSelect;
    std::vector<GEC::UI::Elements::Button*> layerSelect;
    std::vector<GEC::UI::Elements::TextDisplay*> labels;

    GEC::UI::Elements::InputField* brushLayerInput = nullptr;
    int selectedDrawTile = 0;
    int drawLayer = -1, brushLayer = 0;

    GEC::UI::Elements::Checkbox* layerCollidable_input = nullptr;
};


#endif