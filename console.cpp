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

void Console::appendOutput(Font &font, const char output[])
{
    // Add a new mesh to the output array:
    outputText.emplace_back();
    // Initialise the text within the mesh:
    //font.renderString(outputMesh.back(), output);
    // Arbitrarily limit the length of the output list:
    if(outputText.size() > 20)
    {
        // Remove the head of the output list:
        outputText.pop_front();
    }
}

void Console::appendInput(Font &font, const char input[])
{
    // Arbitrary limit on input length:
    if(inputString.length() + strlen(input) <= 100)
    {
        // Append the input to the string buffer:
        inputString += input;
        // Update the graphical mesh:
        inputText.setText(font, FONT_SIZE, inputString.data());
    }
}

void Console::submitInput(Font &font, lua_State *luaState)
{
    // Check that there is something to submit:
    if(inputString.length() > 0)
    {
        // Append the input string to the output:
        appendOutput(font, inputString.data());
        /*
        // Pass the input string to Lua:
        int result = luaL_loadbuffer(luaState, inputString.data(), inputString.length(), "input") || lua_pcall(luaState, 0, 0, 0);
        // Check for Lua errors:
        if(result != 0)
        {
            // Get the error message from the top of the stack:
            const char *errorMessage = lua_tostring(luaState, -1);
            // Append the error message to the console's output:
            appendOutput(font, errorMessage);
        }
        */
        // Clear the input string:
        inputString.clear();
    }
}

void Console::backspace(Font &font)
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
            inputText.setText(font, FONT_SIZE, inputString.data());
        }
    }
}

void Console::draw(const Graphics &graphics)
{
    // Arbitrary offset:
    glm::vec2 offset(0.0f, FONT_SIZE * 2.0f);

    for(TextField &field : outputText)
    {
        // Render one lone line of output:
        field.draw(graphics, offset);
        // Increment the offset by line height:
        offset.y += FONT_SIZE;
    }
    // Check if the input string contains data:
    if(inputString.empty() == false)
    {
        // Only render the input string if not empty:
        inputText.draw(graphics, offset);
    }
}
