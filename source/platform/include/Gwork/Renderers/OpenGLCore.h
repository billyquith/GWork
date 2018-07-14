#ifndef GWORK_RENDERERS_OPENGLCORE_H
#define GWORK_RENDERERS_OPENGLCORE_H


#include <Gwork/BaseRender.h>
#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>
#include <memory>
#include <functional>

namespace Gwk
{
    namespace Renderer
    {
        //
        //! Renderer for [OpenGL](https://www.opengl.org/) Core Profile.
        //
        class GWK_EXPORT OpenGLCore : public Gwk::Renderer::Base
        {
            template<typename T>
            using deleted_unique_ptr = std::unique_ptr<T, std::function<void(T*)>>;

            static const wchar_t BeginCharacter = L' ';    // First Character of Wide Character Table
            static const wchar_t LastCharacter = 0x2FFF;   // Last Character of Wide Character Table
            static const wchar_t NewLineCharacter = L'\n'; // New Line Character

        public:

            OpenGLCore(ResourcePaths& paths, const Gwk::Rect& viewRect);
            virtual ~OpenGLCore();

            void Init() override;

            void Begin() override;
            void End() override;

            void SetDrawColor(Gwk::Color color) override;
            void DrawFilledRect(Gwk::Rect rect) override;

            void StartClip() override;
            void EndClip() override;

            void DrawTexturedRect(const Gwk::Texture& texture, Gwk::Rect targetRect, float u1 = 0.0f,
                                  float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f) override;

            Gwk::Color PixelColor(const Gwk::Texture& texture,
                                  unsigned int x, unsigned int y,
                                  const Gwk::Color& col_default) override;

            void RenderText(const Gwk::Font& font,
                            Gwk::Point pos,
                            const Gwk::String& text) override;

            Gwk::Point MeasureText(const Gwk::Font& font, const Gwk::String& text) override;

            // Resource Loader
            Gwk::Font::Status LoadFont(const Gwk::Font& font) override;
            void FreeFont(const Gwk::Font& font) override;
            bool EnsureFont(const Gwk::Font& font) override;

            Texture::Status LoadTexture(const Gwk::Texture& texture) override;
            void FreeTexture(const Gwk::Texture& texture) override;
            TextureData GetTextureData(const Gwk::Texture& texture) const override;
            bool EnsureTexture(const Gwk::Texture& texture) override;

        protected:// Resourses

            struct GLTextureData : public Gwk::TextureData
            {
                GLTextureData()
                {
                }
                GLTextureData(const GLTextureData&) = delete;
                GLTextureData(GLTextureData&& other)
                    : GLTextureData()
                {
                    std::swap(width, other.width);
                    std::swap(height, other.height);
                    std::swap(readable, other.readable);
                    std::swap(texture_id, other.texture_id);

                    m_ReadData.swap(other.m_ReadData);
                }

                ~GLTextureData();

                unsigned int texture_id;
                deleted_unique_ptr<unsigned char> m_ReadData;
            };

            struct GLFontData
            {
                GLFontData()
                    : m_Spacing(0.f)
                {
                }

                GLFontData(const GLFontData&) = delete;
                GLFontData(GLFontData&& other)
                    : GLFontData()
                {
                    std::swap(width, other.width);
                    std::swap(height, other.height);
                    std::swap(texture_id, other.texture_id);
                    std::swap(baked_chars, other.baked_chars);
                }

                ~GLFontData()
                {
                }

                struct stbtt_bakedchar
                {
                    unsigned short x0, y0, x1, y1; // coordinates of bbox in bitmap
                    float xoff, yoff, xadvance;
                };

                std::vector<stbtt_bakedchar> baked_chars;

                float   m_Spacing;

                int width;
                int height;
                unsigned int texture_id;
            };

            std::unordered_map<Font, GLFontData> m_fonts;
            std::unordered_map<Texture, GLTextureData> m_textures;
            std::pair<const Font, GLFontData>* m_lastFont;
            std::pair<const Texture, GLTextureData>* m_lastTexture;
        public:

            bool InitializeContext(Gwk::WindowProvider* window) override;
            bool ShutdownContext(Gwk::WindowProvider* window) override;
            bool PresentContext(Gwk::WindowProvider* window) override;
            bool ResizedContext(Gwk::WindowProvider* window, int w, int h) override;
            bool BeginContext(Gwk::WindowProvider* window) override;
            bool EndContext(Gwk::WindowProvider* window) override;

            void*   m_context;

        private:

            uint32_t loadShaders(const char *vertex, const char *fragment);

            /**
             * @brief Perform actual rendering.
             */
            void Flush();

            /**
             * @brief Add vertex to buffer.
             * @param x
             * @param y
             * @param u
             * @param v
             */
            void AddVert(int x, int y, float u = 0.0f, float v = 0.0f);
            void SetTexture(unsigned int texture);

            Rect m_viewRect;
            Color m_color;
            unsigned int m_current_texture;
            unsigned char m_textures_on;

            struct Vertex
            {
                glm::vec3 pos;
                glm::vec2 uv;
                glm::vec4 color;
            };

            std::vector<Vertex> m_vertices;

            glm::mat4 m_projectionMatrix;

            uint32_t m_program;
            uint32_t m_activeProgram;

            uint32_t m_vbo;
            uint32_t m_vao;
        };


    }
}

#endif //GWORK_RENDERERS_OPENGLCORE_H
