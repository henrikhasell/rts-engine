#include "console.hpp"

using namespace Engine;
using namespace GL;

Console::Console()
{
    //ctor
}

Console::~Console()
{
    //dtor
}

void Console::appendOutput(const Font &font, const char output[])
{
    // Add a new mesh to the output array:
    outputMesh.emplace_back();
    // Initialise the text within the mesh:
    font.renderString(outputMesh.back(), output);
    // Arbitrarily limit the length of the output list:
    if(outputMesh.size() > 20)
    {
        // Remove the head of the output list:
        outputMesh.pop_front();
    }
}

void Console::appendInput(const Font &font, const char input[])
{
    // Arbitrary limit on input length:
    if(inputString.length() + strlen(input) <= 100)
    {
        // Append the input to the string buffer:
        inputString += input;
        // Update the graphical mesh:
        font.renderString(inputMesh, inputString.data());
    }
}

void Console::submitInput(const Font &font, lua_State *state)
{
    // Check that there is something to submit:
    if(inputString.length() > 0)
    {
        // Append the input string to the output:
        appendOutput(font, inputString.data());
        // Pass the input string to Lua:
        // TODO
        // Clear the input string:
        inputString.clear();
    }
}

void Console::backspace(const Font &font)
{
    // Check that the string has data:
    if(inputString.length() > 0)
    {
        int last;
        // This loop exists to handle
        // multi-byte UTF-8 encoding:
        do
        {
            // Fetch magic number from input string:
            last = inputString.back() & 0x00C0;
            // Remove last character of input string:
            inputString.pop_back();
        } // ... While magic number is equal to 0x80:
        while(last == 0x80);
        // Update the graphical mesh (if appropriate):
        if(inputString.length() > 0)
        {
            font.renderString(inputMesh, inputString.data());
        }
    }
}

void Console::draw(const Graphics &graphics)
{
    // Arbitrary offset:
    glm::vec2 offset(0.0f, 24.0f);

    for(Engine::GL::Mesh2D &mesh : outputMesh)
    {
        // Render one lone line of output:
        mesh.draw(graphics, offset);
        // Increment the offset by line height:
        offset.y += 24.0f;
    }
    // Check if the input string contains data:
    if(inputString.empty() == false)
    {
        // Only render the input string if not empty:
        inputMesh.draw(graphics, offset);
    }
}
