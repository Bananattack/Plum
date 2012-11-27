#include "../plum.h"


namespace plum
{
    namespace
    {
        const char* const libraryName = "plum";

        int exit(lua_State* L)
        {
            int argumentCount = lua_gettop(L);

            if(argumentCount >= 1)
            {
                const char* message = lua_tostring(L, 1);
                if(message)
                {
                    script::instance(L).engine().quit(message);
                }
                else
                {
                    script::instance(L).engine().quit("nil");
                }
            }
            else
            {
                script::instance(L).engine().quit();
            }
            
            return 0;
        }

        int refresh(lua_State* L)
        {
            auto& script = script::instance(L);
            script.engine().refresh(script);
            return 0;
        }

        int setTitle(lua_State* L)
        {
            int argumentCount = lua_gettop(L);

            if(argumentCount >= 1)
            {
                script::instance(L).engine().setTitle(lua_tostring(L, 1));
            }
            
            return 0;
        }

        int rgb(lua_State* L)
        {
            int r = luaL_checkint(L, 1);
            int g = luaL_checkint(L, 2);
            int b = luaL_checkint(L, 3);
            int a = luaL_optint(L, 4, 255);
            lua_pushinteger(L, plum::rgba(r, g, b, a).value);
            return 1;
        }

        int channels(lua_State* L)
        {
            int color = luaL_checkint(L, 1);
            ColorChannel r, g, b, a;
            plum::getRGBA(color, r, g, b, a);
            lua_pushinteger(L, r);
            lua_pushinteger(L, g);
            lua_pushinteger(L, b);
            lua_pushinteger(L, a);
            return 4;
        }

        int hsv(lua_State* L)
        {
            int h = luaL_checkint(L, 1);
            int s = luaL_checkint(L, 2);
            int v = luaL_checkint(L, 3);
            int a = luaL_optint(L, 4, 255);
            lua_pushinteger(L, plum::hsv(h, s, v, a).value);
            return 1;
        }

        int loadConfig(lua_State* L)
        {
            Config cfg;
            cfg.init("plum.cfg", "config", L);
            lua_pushvalue(cfg.lua, -1);
            lua_xmove(cfg.lua, L, 1);
            return 1;
        }

        int hookInput(lua_State* L)
        {
            // Check that first argument is an input object.
            // If so, happily grab the pointer.
            Input** inp = (Input**) luaL_checkudata(L, 1, "plum_input");

            // Get that function on the stack!
            if(!lua_isfunction(L, 2) && !lua_isnil(L, 2))
            {
                luaL_error(L, "Invalid argument #2 to plum.hookInput. Must be a valid function, or nil.");
                return 0;
            }

            // Nuke old callback reference (if any)
            {
                std::vector<Script::InputHook>& hooks(script::instance(L).inputHooks);
                int ref = LUA_NOREF;
                for(uint32_t i = 0; i < hooks.size(); i++)
                {
                    // Okay, convert our reference number input to userdata.
                    Script::InputHook& hook = hooks[i];
                    lua_rawgeti(L, LUA_REGISTRYINDEX, hook.inputRef);
                    Input** p = (Input**) luaL_checkudata(L, -1, "plum_input");

                    // If the passed argument is the same input object,
                    // Then discard that old references.
                    if(*p == *inp)
                    {
                        hooks.erase(hooks.begin() + i);

                        luaL_unref(L, LUA_REGISTRYINDEX, hook.inputRef);
                        luaL_unref(L, LUA_REGISTRYINDEX, hook.callbackRef);
                        break;
                    }
                }
            }
            // Store new callback reference stuff.
            // Unless nil, in which case we just remove old entries.
            // argh.
            if(!lua_isnil(L, 2))
            {
                Script::InputHook hook;

                lua_pushvalue(L, 1);
                hook.inputRef = luaL_ref(L, LUA_REGISTRYINDEX);

                lua_pushvalue(L, 2);
                hook.callbackRef = luaL_ref(L, LUA_REGISTRYINDEX);

                script::instance(L).inputHooks.push_back(hook);
            }
            return 0;
        }

        int unhookAllInput(lua_State* L)
        {
            std::vector<Script::InputHook>& hooks(script::instance(L).inputHooks);
            for(uint32_t i = 0; i < hooks.size(); i++)
            {
                Script::InputHook& hook = hooks[i];
                luaL_unref(L, LUA_REGISTRYINDEX, hook.inputRef);
                luaL_unref(L, LUA_REGISTRYINDEX, hook.callbackRef);
            }
            hooks.clear();
            return 0;
        }

        static void encodeImageData(lua_State* L, Canvas* canvas)
        {
            // Allocate buffer, four ints representing texture dimension stuff.
            // (occupied width, height (occupied size), width, height (overall size))
            // And the rest for compressed data (might not use all of it).
            uint8_t* buffer = new uint8_t[sizeof(uint32_t) * 4 + canvas->width * canvas->height * sizeof(Color)];
            // Pack the dimensions in 4 unsigned 32-bit integers.
            ((uint32_t*) buffer)[0] = (uint32_t) canvas->occupiedWidth;
            ((uint32_t*) buffer)[1] = (uint32_t) canvas->occupiedHeight;
            ((uint32_t*) buffer)[2] = (uint32_t) canvas->width;
            ((uint32_t*) buffer)[3] = (uint32_t) canvas->height;

            //printf("Encoded thing has dimensions (%u, %u, %u, %u)\r\n", ((uint32_t*) buffer)[0], ((uint32_t*) buffer)[1], ((uint32_t*) buffer)[2], ((uint32_t*) buffer)[3]);
            // Compress.
            int compressedSize = Compression::compressData((uint8_t*)(canvas->data),
                canvas->width * canvas->height * sizeof(Color),
                buffer + sizeof(uint32_t) * 4,
                canvas->width * canvas->height * sizeof(Color)
            );
            // Encode.
            std::string encodedText = Base64::encode(std::string(buffer, buffer + sizeof(uint32_t) * 4 + compressedSize));
            // Push encoded text.
            lua_pushstring(L, encodedText.c_str());
            // Destroy buffer.
            delete buffer;
        }

        int encodeCanvas(lua_State* L)
        {
            auto canvas = script::ptr<Canvas>(L, 1);
            encodeImageData(L, canvas);

            return 1;
        }

        int encodeImage(lua_State* L)
        {
            auto image = script::ptr<Image>(L, 1);
            encodeImageData(L, image->getCanvas());

            return 1;
        }

        static Canvas* decodeImageData(const char* s)
        {
            std::string blob = Base64::decode(s);
            uint8_t* data = (uint8_t*) blob.data();

            // Unpack the dimensions from 4 unsigned 32-bit integers.
            uint32_t occupiedWidth = ((uint32_t*) data)[0];
            uint32_t occupiedHeight = ((uint32_t*) data)[1];
            uint32_t width = ((uint32_t*) data)[2];
            uint32_t height = ((uint32_t*) data)[3];
            // Allocate image.
            Canvas* canvas = new Canvas(width, height);
            canvas->occupiedWidth = occupiedWidth;
            canvas->occupiedHeight = occupiedHeight;
            canvas->restoreClipRegion();
            // Decompress.
            data += sizeof(uint32_t) * 4;
            Compression::decompressData(data, blob.length(), (uint8_t*)(canvas->data), canvas->width * canvas->height * sizeof(Color));
            // Done!
            return canvas;
        }

        int decodeCanvas(lua_State* L)
        {
            const char* s = luaL_checkstring(L, 1);
            Canvas* canvas = decodeImageData(s);
            // Push decoded image.
            script::push(L, canvas, LUA_NOREF);
            return 1;
        }

        int decodeImage(lua_State* L)
        {
            const char* s = luaL_checkstring(L, 1);
            Canvas* canvas = decodeImageData(s);
            // Push decoded texture.
            script::push(L, new Image(canvas), LUA_NOREF);
            // Destroy temporary image.
            delete canvas;
            return 1;
        }

        int encodeTilemap(lua_State* L)
        {
            auto tilemap = script::ptr<Tilemap>(L, 1);

            // Allocate buffer, 2 ints representing texture dimension stuff.
            // (width, height (overall size))
            // And the rest for compressed data (might not use all of it).
            uint8_t* buffer = new uint8_t[sizeof(uint32_t) * 2 + tilemap->width * tilemap->height * sizeof(Tile)];
            // Pack the dimensions in 2 unsigned 32-bit integers.
            ((uint32_t*) buffer)[0] = (uint32_t) tilemap->width;
            ((uint32_t*) buffer)[1] = (uint32_t) tilemap->height;

            // Compress.
            int compressedSize = Compression::compressData((uint8_t*)(tilemap->data),
                tilemap->width * tilemap->height * sizeof(Tile),
                buffer + sizeof(uint32_t) * 2,
                tilemap->width * tilemap->height * sizeof(Tile)
            );
            // Encode.
            std::string encodedText = Base64::encode(std::string(buffer, buffer + sizeof(uint32_t) * 2 + compressedSize));
            // Push encoded text.
            lua_pushstring(L, encodedText.c_str());
            // Destroy buffer.
            delete buffer;

            return 1;
        }

        static int decodeTilemap(lua_State* L)
        {
            const char* s = luaL_checkstring(L, 1);
            std::string blob = Base64::decode(s);
            uint8_t* data = (uint8_t*) blob.data();

            // Unpack the dimensions from 2 unsigned 32-bit integers.
            uint32_t width = ((uint32_t*) data)[0];
            uint32_t height = ((uint32_t*) data)[1];
            // Allocate image.
            Tilemap* tilemap = new Tilemap(width, height);
            // Decompress.
            data += sizeof(uint32_t) * 2;
            Compression::decompressData(data, blob.length(), (uint8_t*)(tilemap->data), tilemap->width * tilemap->height * sizeof(Tile));
            // Push decoded tilemap.
            script::push(L, tilemap, LUA_NOREF);
            return 1;
        }

        int digestString(lua_State* L)
        {
            // Get the SHA1 digest of the string argument passed.
            std::string s = luaL_checkstring(L, 1);
            std::string result;
            SHA1::digestString(s, result);
            lua_pushstring(L, Base64::encode(result).c_str());
            return 1;
        }

        int digestFile(lua_State* L)
        {
            // Get the SHA1 digest of the file named by the string argument passed.
            std::string s = luaL_checkstring(L, 1);
            std::string result;
            SHA1::digestFile(s, result);
            lua_pushstring(L, Base64::encode(result).c_str());
            return 1;
        }

        int vergeReadTilemap8(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);
            int width = luaL_checkinteger(L, 2);
            int height = luaL_checkinteger(L, 3);

            int blockSize = width * height;
            uint8_t* buffer = new uint8_t[blockSize];
            bool success = file->readVergeCompressed(buffer, blockSize);

            if(success)
            {
                Tilemap* tilemap = new Tilemap(width, height);

                // Read bytes.
                for(int t = 0; t < width * height; t++)
                {
                    tilemap->data[t] = buffer[t];
                }

                // Push tilemap.
                script::push(L, tilemap, LUA_NOREF);
            }

            delete [] buffer;
            return success;
        }

        int vergeReadTilemap16(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);
            int width = luaL_checkinteger(L, 2);
            int height = luaL_checkinteger(L, 3);

            int blockSize = width * height * 2;
            uint8_t* buffer = new uint8_t[blockSize];
            bool success = file->readVergeCompressed(buffer, blockSize);

            if(success)
            {
                Tilemap* tilemap = new Tilemap(width, height);

                // Read little-endian 16-bit ints.
                for(int t = 0; t < width * height; t++)
                {
                    tilemap->data[t] = buffer[t * 2 + 1] << 8 | buffer[t * 2];
                }

                // Push tilemap.
                script::push(L, tilemap, LUA_NOREF);
            }

            delete [] buffer;
            return success;
        }

        int vergeReadObstructionCanvas(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);
            int width = luaL_checkinteger(L, 2);
            int height = luaL_checkinteger(L, 3);

            int blockSize = width * height;
            uint8_t* buffer = new uint8_t[blockSize];
            bool success = file->readVergeCompressed(buffer, blockSize);

            if(success)
            {
                Canvas* canvas = new Canvas(width, height);

                // Read bytes. Verge stores one pixel per byte, no bitwise packing.
                for(int t = 0; t < width * height; t++)
                { 
                    // White = obs, transparent = nothing
                    canvas->data[t] = (buffer[t] != 0) * Color::White;
                }
                // Push image.
                script::push(L, canvas, LUA_NOREF);
            }

            delete [] buffer;
            return success;
        }

        int vergeReadRGBCanvas(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);
            int width = luaL_checkinteger(L, 2);
            int height = luaL_checkinteger(L, 3);

            int blockSize = width * height * 3;
            uint8_t* buffer = new uint8_t[blockSize];
            bool success = file->readVergeCompressed(buffer, blockSize);

            if(success)
            {
                Canvas* canvas = new Canvas(width, height);

                // Read bytes.
                for(int t = 0; t < width * height; t++)
                { 
                    // Pixels are stored in RGB format.
                    canvas->data[t] = Color(buffer[t * 3], buffer[t * 3 + 1], buffer[t * 3 + 2]);
                }

                // Push image.
                script::push(L, canvas, LUA_NOREF);
            }

            delete [] buffer;
            return success;
        }
    }

    namespace script
    {
        void initLibrary(lua_State* L)
        {
            const luaL_Reg functions[] = {
                {"exit", exit},
                {"refresh", refresh},
                {"setTitle", setTitle},
                {"loadConfig", loadConfig},
                {"hookInput", hookInput},
                {"unhookAllInput", unhookAllInput},
                {nullptr, nullptr},
            };
            luaL_register(L, libraryName, functions);

            // Create the 'data' table.
            lua_newtable(L);
            lua_pushvalue(L, -1);
            lua_setfield(L, -3, "data");

            lua_pushcfunction(L, encodeCanvas);
            lua_setfield(L, -2, "encodeCanvas");
            lua_pushcfunction(L, decodeCanvas);
            lua_setfield(L, -2, "decodeCanvas");
            lua_pushcfunction(L, encodeImage);
            lua_setfield(L, -2, "encodeImage");
            lua_pushcfunction(L, decodeImage);
            lua_setfield(L, -2, "decodeImage");
            lua_pushcfunction(L, encodeTilemap);
            lua_setfield(L, -2, "encodeTilemap");
            lua_pushcfunction(L, decodeTilemap);
            lua_setfield(L, -2, "decodeTilemap");
            lua_pushcfunction(L, digestString);
            lua_setfield(L, -2, "digestString");
            lua_pushcfunction(L, digestFile);
            lua_setfield(L, -2, "digestFile");

            // Done with 'data' now.
            lua_pop(L, 1);

            // Create the 'color' table.
            lua_newtable(L);
            lua_pushvalue(L, -1);
            lua_setfield(L, -3, "color");

            lua_pushinteger(L, Color::White);
            lua_setfield(L, -2, "White");
            lua_pushinteger(L, Color::Red);
            lua_setfield(L, -2, "Red");
            lua_pushinteger(L, Color::Green);
            lua_setfield(L, -2, "Green");
            lua_pushinteger(L, Color::Blue);
            lua_setfield(L, -2, "Blue");
            lua_pushinteger(L, Color::Magenta);
            lua_setfield(L, -2, "Magenta");
            lua_pushinteger(L, Color::Cyan);
            lua_setfield(L, -2, "Cyan");
            lua_pushinteger(L, Color::Yellow);
            lua_setfield(L, -2, "Yellow");
            lua_pushinteger(L, Color::Black);
            lua_setfield(L, -2, "Black");

            lua_pushcfunction(L, rgb);
            lua_setfield(L, -2, "rgb");

            lua_pushcfunction(L, hsv);
            lua_setfield(L, -2, "hsv");

            lua_pushcfunction(L, channels);
            lua_setfield(L, -2, "channels");

            // Done with 'color' now.
            lua_pop(L, 1);

            // Create the 'blend' table.
            lua_newtable(L);
            lua_pushvalue(L, -1);
            lua_setfield(L, -3, "blend");

            lua_pushinteger(L, BlendOpaque);
            lua_setfield(L, -2, "Opaque");
            lua_pushinteger(L, BlendMerge);
            lua_setfield(L, -2, "Merge");
            lua_pushinteger(L, BlendMerge);
            lua_setfield(L, -2, "Alpha");
            lua_pushinteger(L, BlendPreserve);
            lua_setfield(L, -2, "Preserve");
            lua_pushinteger(L, BlendAdd);
            lua_setfield(L, -2, "Add");
            lua_pushinteger(L, BlendSubtract);
            lua_setfield(L, -2, "Subtract");

            // Done with 'blend' now.
            lua_pop(L, 1);


            // Create the 'verge' table.
            lua_newtable(L);
            lua_pushvalue(L, -1);
            lua_setfield(L, -3, "verge");

            lua_pushcfunction(L, vergeReadTilemap8);
            lua_setfield(L, -2, "readTilemap8");
            lua_pushcfunction(L, vergeReadTilemap16);
            lua_setfield(L, -2, "readTilemap16");
            lua_pushcfunction(L, vergeReadObstructionCanvas);
            lua_setfield(L, -2, "readObstructionCanvas");
            lua_pushcfunction(L, vergeReadRGBCanvas);
            lua_setfield(L, -2, "readRGBCanvas");

            // Done with 'verge' now.
            lua_pop(L, 1);

            // Pop the library.
            lua_pop(L, 1);

            // Load all the submodule and object definitions contained within Plum.
            initVideoModule(L);
            initTimerModule(L);

            initInputObject(L);
            initKeyboardModule(L);
            initMouseModule(L);

            initFontObject(L);
            initSoundObject(L);
            initSongObject(L);
            initFileObject(L);
            initFileObject(L);
            initDirectoryObject(L);
            initCanvasObject(L);
            initPointObject(L);
            initRectObject(L);
            initTransformObject(L);
            initImageObject(L);
            initSpritesheetObject(L);
            initFontObject(L);
            initTilemapObject(L);
        }
    }
}