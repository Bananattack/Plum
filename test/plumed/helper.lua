-- Setup base Object class for class system.
do
	local o = { class_name = 'Object', __init = function(self) end }
	local meta = {}
	function meta:__call()
		local t = { class_name = o.class_name }
		return t
	end
	
	plumed.Object = o
	setmetatable(plumed.Object, meta)
end

function plumed.module(target)
    local this_module = target or {}
    -- Create a global class given a name string and a parent class reference.
    function this_module.class(name, parent)
        local cls = {}
        local meta = {}
        local getter = {}
        local setter = {}
        
        cls.class_name = name
        cls.parent = parent or plumed.Object
        
        function cls:__index(key)
            local f = getter[key]
            return (f and f(self)) or cls[key] or rawget(self, key)
        end
        
        function cls:__newindex(key, value)
            local f = setter[key]
            if key == 'parent' then
                error('The parent class may not be modified after construction.', 2)
            elseif f then
                f(self, value)
            elseif cls[key] then
                cls[key] = value
            else
                rawset(self, key, value)
            end
            
        end
        
        -- A helper for creating getter/setter things
        function cls._property(key, get, set)
            getter[key] = get
            setter[key] = set
        end
        
        -- An indexer on the class that looks up its parents on fallback
        function meta:__index(key)
            return rawget(self, key) or rawget(self, 'parent')[key]
        end

        local function construct(cls, obj, arg)
            local called_super = (cls.parent == plumed.Object)
            local function call_super(...)
                local p = cls.parent
                
                called_super = true
                local t = { ... }
                if p then
                    construct(p, obj, t)
                else
                    -- Invoking super in class where not necessary, whatever...
                    return
                end
            end
        
            -- Make 'super' a temporary global to mean the constructor of the parent class.
            local tmp = rawget(_G, 'super')
            rawset(_G, 'super', call_super)

            if arg then
                cls.__init(obj, unpack(arg))
            end
            
            -- Restore old 'super' meaning.
            rawset(_G, 'super', tmp)
            
            -- Yell if the super constructor wasn't called
            -- for classes that don't subclass plumed.Object
            if not called_super then
                error("super() must be invoked in constructor for class '" .. cls.class_name .. "'.")
            end
        end

        
        -- The external constructor call.
        function meta:__call(...)
            -- Create a base table
            local obj = {}
            -- Setup class information
            setmetatable(obj, cls)
            
            local t = { ... }
            construct(cls, obj, t)
            
            -- Return the finished constructor
            return obj
        end
        
        setmetatable(cls, meta)
        
        -- Shove into global namespace
        this_module[name] = cls
    
        -- For convenience so you can write the Python-like: class 'Entity'(Sprite)
        -- instead of the lamer class('Entity', Sprite)
        -- Treat class('Entity', Sprite)(OtherThing) by ignoring last term.
        return function(p) cls.parent = parent or p or plumed.Object end
    end
    
    return this_module
end

-- This piece of magic tacks the "class" function to this module.
plumed.module(plumed)


function plumed.methodPointer(self, meth)
    return function(...)
        return meth(self, ...)
    end
end
