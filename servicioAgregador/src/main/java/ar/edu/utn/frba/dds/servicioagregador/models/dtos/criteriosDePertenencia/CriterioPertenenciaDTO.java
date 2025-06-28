package ar.edu.utn.frba.dds.servicioagregador.models.dtos.criteriosDePertenencia;

import com.fasterxml.jackson.annotation.JsonSubTypes;
import com.fasterxml.jackson.annotation.JsonTypeInfo;
import lombok.Data;

@JsonTypeInfo(
    use = JsonTypeInfo.Id.NAME,
    include = JsonTypeInfo.As.PROPERTY,
    property = "tipo"
)
@JsonSubTypes({
    @JsonSubTypes.Type(value = CriterioCategoriaDTO.class, name = "categoria"),
    @JsonSubTypes.Type(value = CriterioFechaDTO.class, name = "fecha"),
    @JsonSubTypes.Type(value = CriterioUbicacionDTO.class, name = "ubicacion")
})
@Data
public abstract class CriterioPertenenciaDTO {
  private String tipo;
}