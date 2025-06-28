package ar.edu.utn.frba.dds.servicioagregador.services.impl;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Categoria;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.Coleccion;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.Etiqueta;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.Hecho;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.criteriosPertenencia.CriterioCategoria;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.criteriosPertenencia.CriterioDePertenencia;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.criteriosPertenencia.CriterioFecha;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.criteriosPertenencia.CriterioUbicacion;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.criteriosPertenencia.ICriterioDePertenencia;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes.Archivo;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes.Fuente;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes.FuenteDinamica;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes.FuenteEstatica;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes.FuenteProxy;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.ubicacion.Coordenadas;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.ubicacion.Ubicacion;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.FuenteDTO;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.criteriosDePertenencia.CriterioCategoriaDTO;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.criteriosDePertenencia.CriterioFechaDTO;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.criteriosDePertenencia.CriterioPertenenciaDTO;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.criteriosDePertenencia.CriterioUbicacionDTO;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.input.ColeccionInputDTO;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.output.ColeccionOutputDTO;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.output.HechoOutputDTO;
import ar.edu.utn.frba.dds.servicioagregador.models.repositories.ICategoriaRepository;
import ar.edu.utn.frba.dds.servicioagregador.models.repositories.IColeccionRepository;
import ar.edu.utn.frba.dds.servicioagregador.models.repositories.IEtiquetaRepository;
import ar.edu.utn.frba.dds.servicioagregador.models.repositories.IHechoRepository;
import ar.edu.utn.frba.dds.servicioagregador.services.IColeccionService;
import ar.edu.utn.frba.dds.servicioagregador.services.IHechoService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import reactor.core.publisher.Mono;


import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

@Service
public class ColeccionService implements IColeccionService {
  @Autowired
  private IHechoService hechoService;
  @Autowired
  private IColeccionRepository coleccionRepository;
  @Autowired
  private IHechoRepository hechoRepository;
  @Autowired
  private ICategoriaRepository categoriaRepository;
  @Autowired
  private IEtiquetaRepository etiquetaRepository;

  @Override
  public ColeccionOutputDTO crearColeccion(ColeccionInputDTO coleccionDTO) {
    // Crea colección
    var coleccionCreada = new Coleccion(coleccionDTO.getNombre());
    coleccionCreada.setDescripcion(coleccionDTO.getDescripcion());

    // Crea criterio
    var criterioDePertenencia = new CriterioDePertenencia();
    coleccionDTO.getCriterios()
        .forEach(criterio -> criterioDePertenencia.agregarCriterio(this.mapearCriterio(criterio)));
    coleccionCreada.setCriterioDePertenencia(criterioDePertenencia);

    // Encuentra y agrega las fuentes
    coleccionDTO.getFuentes()
        .stream()
        .map(this::mapearFuente)
        .forEach(coleccionCreada::agregarFuente);

    // Carga los hechos de las fuentes
    coleccionCreada.agregarHechosDeFuente();

    // Guarda en sus repos y retorna el dto
    coleccionCreada.getHechos().forEach(hechoRepository::save);
    this.coleccionRepository.save(coleccionCreada);
    return this.coleccionOutputDto(coleccionCreada);
  }

  @Override
  public ColeccionOutputDTO obtenerColeccionPorId(String id) {
    return this.coleccionOutputDto(this.coleccionRepository.findById(id));
  }

  @Override
  public List<HechoOutputDTO> obtenerHechosDeColeccionPorId(String id) {
    return this.coleccionRepository
        .findById(id)
        .getHechos()
        .stream()
        .map(this::hechoToDto)
        .collect(Collectors.toList());
  }



  // COLECCION -> OUTPUT DTO
  private ColeccionOutputDTO coleccionOutputDto(Coleccion coleccion) {
    ColeccionOutputDTO coleccionOutputDTO = new ColeccionOutputDTO();
    coleccionOutputDTO.setId(coleccion.getId());
    coleccionOutputDTO.setNombre(coleccion.getTitulo());
    coleccionOutputDTO.setDescripcion(coleccion.getDescripcion());
    coleccionOutputDTO.setCriterios(this.criteriosToDto(coleccion.getCriterioDePertenencia()));
    coleccionOutputDTO.setFuentes(this.fuentesToDto(coleccion.getFuentes()));
    return coleccionOutputDTO;
  }

  private List<CriterioPertenenciaDTO> criteriosToDto(CriterioDePertenencia criterioDePertenencia) {
    List<CriterioPertenenciaDTO> criteriosDto = new ArrayList<>();
    for (ICriterioDePertenencia criterio : criterioDePertenencia.getCriterios()) {
      if (criterio instanceof CriterioCategoria criterioCat) {
        CriterioCategoriaDTO categoriaDto = new CriterioCategoriaDTO();
        categoriaDto.setCategoriaId(criterioCat.getCategoriaId());
        categoriaDto.setTipo("categoria");
        criteriosDto.add(categoriaDto);
      } else if (criterio instanceof CriterioFecha criterioFecha) {
        CriterioFechaDTO fechaDto = new CriterioFechaDTO();
        fechaDto.setTipo("fecha");
        fechaDto.setFechaInicial(criterioFecha.getFechaInicial().toString());
        fechaDto.setFechaFinal(criterioFecha.getFechaFinal().toString());
        criteriosDto.add(fechaDto);
      } else if (criterio instanceof CriterioUbicacion criterioUbicacion) {
        CriterioUbicacionDTO ubicacionDto = new CriterioUbicacionDTO();
        ubicacionDto.setTipo("ubicacion");
        ubicacionDto.setLatitud(criterioUbicacion.getLatitud());
        ubicacionDto.setLongitud(criterioUbicacion.getLongitud());
        criteriosDto.add(ubicacionDto);
      } else {
        throw new IllegalArgumentException("Formato de criterio incorrecto.");
      }
    }
    return criteriosDto;
  }

  private List<FuenteDTO> fuentesToDto(List<Fuente> fuentes) {
    return fuentes
        .stream()
        .map(this::fuenteToDto)
        .collect(Collectors.toList());
  }

  private FuenteDTO fuenteToDto(Fuente fuente) {
    FuenteDTO fuenteDto = new FuenteDTO();
    fuenteDto.setTipo(fuente.getTipo());
    if (fuente instanceof FuenteEstatica && ((FuenteEstatica) fuente).getArchivo() != null) {
      fuenteDto.setArchivo(((FuenteEstatica) fuente).getArchivo().getNombre());
    } else if (fuente instanceof FuenteProxy) {
      fuenteDto.setProxy(((FuenteProxy) fuente).getProxy());
    }
    return fuenteDto;
  }

  private List<HechoOutputDTO> hechosToDto(List<Hecho> hechos) {
    return hechos.stream().map(this::hechoToDto).collect(Collectors.toList());
  }

  private HechoOutputDTO hechoToDto(Hecho hecho) {
    HechoOutputDTO hechoOutputDTO = new HechoOutputDTO();
    hechoOutputDTO.setId(hecho.getId());
    hechoOutputDTO.setTitulo(hecho.getTitulo());
    hechoOutputDTO.setDescripcion(hecho.getDescripcion());
    hechoOutputDTO.setCategoriasId(hecho.getCategorias().stream().map(Categoria::getId).collect(Collectors.toList()));
    hechoOutputDTO.setLatitud(hecho.getUbicacion().getLatitud());
    hechoOutputDTO.setLongitud(hecho.getUbicacion().getLongitud());
    hechoOutputDTO.setFechaAcontecimiento(hecho.getFechaAcontecimiento());
    hechoOutputDTO.setFechaDeCarga(hecho.getFechaDeCarga());
    hechoOutputDTO.setEtiquetasId(hecho.getEtiquetas().stream().map(Etiqueta::getId).collect(Collectors.toList()));
    return hechoOutputDTO;
  }


  // INPUT DTO -> COLECCION
  private ICriterioDePertenencia mapearCriterio(CriterioPertenenciaDTO criterioDTO) {
    if (criterioDTO instanceof CriterioCategoriaDTO catDTO) {
      return new CriterioCategoria(this.categoriaRepository.findById(catDTO.getCategoriaId()));
    } else if (criterioDTO instanceof CriterioFechaDTO fechaDTO) {
      return new CriterioFecha(LocalDateTime.parse(fechaDTO.getFechaInicial(), DateTimeFormatter.ofPattern("dd-MM-yyyy")),
          LocalDateTime.parse(fechaDTO.getFechaFinal(), DateTimeFormatter.ofPattern("dd-MM-yyyy")));
    } else if (criterioDTO instanceof CriterioUbicacionDTO ubiDTO) {
      return new CriterioUbicacion(new Ubicacion(new Coordenadas(ubiDTO.getLatitud(), ubiDTO.getLongitud())));
    } else {
      throw new IllegalArgumentException("Formato de criterio incorrecto.");
    }
  }

  private Fuente mapearFuente(FuenteDTO dto) {
    return switch (dto.getTipo()) {
      case "estatica" ->
          dto.getArchivo() != null
              ? new FuenteEstatica(Archivo.builder().nombre(dto.getArchivo()).build())
              : new FuenteEstatica();
      case "dinamica" -> new FuenteDinamica();
      case "proxy"    -> new FuenteProxy(dto.getProxy());
      default         -> throw new IllegalArgumentException("Tipo de fuente inválido");
    };
  }




  // REFRESCO COLECCIONES CADA 1 HORA
  @Override
  public Mono<Void> refrescarColecciones() {

    this.refrescarPorCriterios();

    FuenteDinamica fuenteDinamica = new FuenteDinamica();
    FuenteEstatica fuenteEstatica = new FuenteEstatica();

    refrescarEn(fuenteDinamica);
    refrescarEn(fuenteEstatica);

    return Mono.empty();
  }

  private void refrescarEn(Fuente fuente) {
    List<Hecho> hechosActualizados = this.hechoService.obtenerHechosRecientes(fuente);
    this.hechoRepository.saveVarios(hechosActualizados);
    this.coleccionRepository.findByFuente(fuente)
        .forEach(coleccion -> {
          if (hechosActualizados != null) {
            hechosActualizados.forEach(coleccion::agregarHecho);
            this.coleccionRepository.save(coleccion);
          }
        });
  }

  private void refrescarPorCriterios() {
    this.coleccionRepository.findByCambioCriterio().forEach(coleccion -> {
      coleccion.revalidarPorCriterio();
      this.coleccionRepository.save(coleccion);
    });
  }
}

